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

bool UBaseAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// EffectContextHandle.Get returns a const, if we are static castings we need to store the result in a const, if we want to store it in a non const we would have to const_cast to cast away the constness
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))  // regular c++ compile time static cast
	{
		return BaseEffectContext->IsBlockedHit();
	}
	return false;
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

bool UBaseAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothAreEnemies || bBothArePlayers;
	return !bFriends;
}

