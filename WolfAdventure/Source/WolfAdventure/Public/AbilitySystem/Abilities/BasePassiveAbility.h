// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BasePassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBasePassiveAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

	// triggers as soon as the ability becomes active
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// callback function
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};
