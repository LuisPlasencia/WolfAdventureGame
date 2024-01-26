// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */)  // not dynamic since our widgets are not going to bind to this delegate but the widget controller (no need for blueprint so no need for addDynamic)

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

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	
	// we make this callback function a client RPC since, OnGameplayEffectAppliedDelegateToSelf is only called on server and not on clients
	// client RPC (Remote procedure call): it will be called on the server but also executed on owning clients, Reliable makes it guaranteed to reach the client even in the case of packet loss (need of confirmation)
	UFUNCTION(Client, Reliable)  
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
