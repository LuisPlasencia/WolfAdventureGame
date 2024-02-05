// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "BaseAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	// we override this to set our own custom Gameplay Effect Context to use with the MakeEffectContext function and the Ability System Component (what class is used to instantiate a gameplay effect context handle object)
	// we specify this abilitySystemGlobals class in the config file DefaultGame.ini 
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
