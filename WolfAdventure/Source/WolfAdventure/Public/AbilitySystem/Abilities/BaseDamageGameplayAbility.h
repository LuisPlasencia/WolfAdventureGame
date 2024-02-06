// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// we set this in the blueprint with a data asset with differente curves depending on the type of damage so we have different values for different ability levels
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
