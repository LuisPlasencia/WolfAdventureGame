// Made by Luis Plasencia Pulido


#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include <UI/HUD/BaseHUD.h>
#include <UI/WidgetController/BaseWidgetController.h>
#include <Player/WolfPlayerState.h>
#include <Kismet/GameplayStatics.h>
#include <Game/WolfAdventureGameModeBase.h>
#include "AbilitySystemComponent.h"
#include <BaseAbilityTypes.h>
#include <Interaction/CombatInterface.h>
#include "BaseGameplayTags.h"
#include "Game/LoadScreenSaveGame.h"
#include <AbilitySystemBlueprintLibrary.h>

bool UBaseAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParamms, ABaseHUD*& OutBaseHUD)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		OutBaseHUD = Cast<ABaseHUD>(PC->GetHUD());
		if (OutBaseHUD)
		{
			AWolfPlayerState* PS = PC->GetPlayerState<AWolfPlayerState>(); 	// it takes an input template parameter
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParamms.AttributeSet = AS;
			OutWCParamms.AbilitySystemComponent = ASC;
			OutWCParamms.PlayerState = PS;
			OutWCParamms.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UBaseAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABaseHUD* BaseHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BaseHUD))
	{
		return BaseHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UBaseAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABaseHUD* BaseHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BaseHUD))
	{
		return BaseHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UBaseAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABaseHUD* BaseHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BaseHUD))
	{
		return BaseHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UBaseAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UBaseAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	// The spec handle is created with the SOURCE ASC and we associate magnitude to tags in this spec handle!
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveGame->Vigor);

	// The spec handle created with the source ASC is then APPLIED to the ASC of the TARGET as a Gameplay Effect!
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data); // dereferencing the wrapper will also give you the dereferenced value inside of it (no need for Get())

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void UBaseAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		// these common abilities wont change depending on level by design so we are not concerned about its level (things like death or hit react)
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			// these are character specific abilities and they change depending on the level of the character
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UBaseAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);

}

void UBaseAbilitySystemLibrary::SetIsRadialDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;


}

void UBaseAbilitySystemLibrary::SetKnockbackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}

}

void UBaseAbilitySystemLibrary::SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}

	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UBaseAbilitySystemLibrary::SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}


UCharacterClassInfo* UBaseAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AWolfAdventureGameModeBase* BaseGameMode = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BaseGameMode == nullptr) return nullptr;

	return BaseGameMode->CharacterClassInfo;
}

UAbilityInfo* UBaseAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AWolfAdventureGameModeBase* BaseGameMode = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BaseGameMode == nullptr) return nullptr;

	return BaseGameMode->AbilityInfo;
}

ULootTiers* UBaseAbilitySystemLibrary::GetLootTiers(const UObject* WorldContextObject)
{
	const AWolfAdventureGameModeBase* BaseGameMode = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BaseGameMode == nullptr) return nullptr;

	return BaseGameMode->LootTiers;
}

bool UBaseAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// EffectContextHandle.Get returns a const, if we are static castings we need to store the result in a const, if we want to store it in a non const we would have to const_cast to cast away the constness
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->IsBlockedHit();
	}
	return false;
}

bool UBaseAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get())) 
	{
		return BaseEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UBaseAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UBaseAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UBaseAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UBaseAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		if (BaseEffectContext->GetDamageType().IsValid())
		{
			// we dereference the pointer
			return *BaseEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UBaseAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UBaseAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UBaseAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// EffectContextHandle.Get returns a const since we are passing in a const parameter variable, if we are static castings we need to store the result in a const, if we want to store it in a non const we would have to const_cast to cast away the constness
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->IsCriticalHit();
	}
	return false;
}

bool UBaseAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->IsRadialDamage();
	}
	return false;
}

float UBaseAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UBaseAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UBaseAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UBaseAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UBaseAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UBaseAbilitySystemLibrary::SetIsSuccessfullDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UBaseAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetDebuffDamage(InDamage);
	}
}

void UBaseAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetDebuffDuration(InDuration);
	}
}

void UBaseAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UBaseAbilitySystemLibrary::SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		// we create an fgameplaytag pointer from a const reference pointer
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		BaseEffectContext->SetDamageType(DamageType);
	}
}

void UBaseAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UBaseAbilitySystemLibrary::SetKnockbackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetKnockbackForce(InForce);
	}
}

void UBaseAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UBaseAbilitySystemLibrary::SetRadialDamageInnerRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UBaseAbilitySystemLibrary::SetRadialDamageOuterRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UBaseAbilitySystemLibrary::SetRadialDamageOrigin(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UBaseAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			// for Implements we pass in UCombatInterface, for Cast we would have passed ICombatInterface
			// we dont check is dead unless we make it pass the first check which is the actor must have a combatinterface
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				// we could just addUnique Overlap.GetActor(), this is too roundabout
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}


}

void UBaseAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	// we need a non const copy of actors so we can modify it
	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		// maximum double number 
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool UBaseAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothAreEnemies || bBothArePlayers;
	return !bFriends;
}

FGameplayEffectContextHandle UBaseAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	
	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);

	// The spec handle is created with the SOURCE ASC and we associate magnitude to tags in this spec handle!
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	// The spec handle created with the source ASC is then APPLIED to the ASC of the TARGET as a Gameplay Effect!
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data); // dereferencing the wrapper will also give you the dereferenced value inside of it (no need for Get())

	return EffectContextHandle;
}

TArray<FRotator> UBaseAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UBaseAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

