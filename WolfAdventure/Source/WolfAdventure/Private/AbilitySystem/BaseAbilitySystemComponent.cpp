// Made by Luis Plasencia Pulido


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include <AbilitySystem/Abilities/BaseGameplayAbility.h>

void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	// we make this callback function a client RPC since, OnGameplayEffectAppliedDelegateToSelf is only called on server and not on clients
	// client RPC (Remote procedure call): it will be called on the server but also executed on owning clients
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilitySystemComponent::ClientEffectApplied);  // not add dynamic since it is not a dynamic delegate
	


//	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
}

void UBaseAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
		{
			// dynamic ability tags are designed to be added and removed at RUNTIME
			AbilitySpec.DynamicAbilityTags.AddTag(BaseAbility->StartupInputTag);
			// give ability can accept const abilitySpec but giveabilityandactivateonce only accepts non-const
			//GiveAbilityAndActivateOnce(AbilitySpec);
			GiveAbility(AbilitySpec);
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// we tell the ability that its input is being pressed
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				// we try to activate the ability, since it may be blocked by other abilities
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	// the ability should determine if it should be cancelled or ended when the ability is no longer pressed

	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// we tell the ability that its input is being released
			AbilitySpecInputReleased(AbilitySpec);
		}
	}

}

void UBaseAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
//	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);

	//for (const FGameplayTag& Tag : TagContainer)  // a reference so we dont copy the tag (+efficiency)
	//{
	//	// TODO: Broadcast the tag to the Widget Controller
	//	const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	//	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
	//}
}
