// Made by Luis Plasencia Pulido


#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include <UI/HUD/BaseHUD.h>
#include <UI/WidgetController/BaseWidgetController.h>
#include <Player/WolfPlayerState.h>
#include <Kismet/GameplayStatics.h>
#include <Game/WolfAdventureGameModeBase.h>
#include "AbilitySystemComponent.h"
#include <BaseAbilityTypes.h>

UOverlayWidgetController* UBaseAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(PC->GetHUD()))
		{
			AWolfPlayerState* PS = PC->GetPlayerState<AWolfPlayerState>(); 	// it takes an input template parameter
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return BaseHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UBaseAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(PC->GetHUD()))
		{
			AWolfPlayerState* PS = PC->GetPlayerState<AWolfPlayerState>(); 	// it takes an input template parameter
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return BaseHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
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

void UBaseAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommmonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UBaseAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AWolfAdventureGameModeBase* BaseGameMode = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BaseGameMode == nullptr) return nullptr;

	return BaseGameMode->CharacterClassInfo;
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
