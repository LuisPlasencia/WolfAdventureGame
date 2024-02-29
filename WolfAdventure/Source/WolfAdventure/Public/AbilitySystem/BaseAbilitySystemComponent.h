// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);  // not dynamic since our widgets are not going to bind to this delegate but the widget controller (no need for blueprint so no need for addDynamic)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven); // multicast lets us bind from multiple different classes (just in case), in this case we want the widget controller to pick it up whenever we give our abilities
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);  // we dont need it to be dynamic or multicast since we are just going to use it in c++ in one class 

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	// when out widget controller calls this function we may be on a client or on the server, we want to make sure that if we are on the client, we send an RPC up to the server so it knows
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	// server RPC so that clients can inform the server that the user has pressed the button and changed the attribute points
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	virtual void OnRep_ActivateAbilities() override;
	
	// we make this callback function a client RPC since, OnGameplayEffectAppliedDelegateToSelf is only called on server and not on clients
	// client RPC (Remote procedure call): it will be called on the server but also executed on owning clients, Reliable makes it guaranteed to reach the client even in the case of packet loss (need of confirmation)
	UFUNCTION(Client, Reliable)  
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
