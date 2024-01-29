// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseProjectileSpell.generated.h"

class ABaseProjectile;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseProjectileSpell : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseProjectile> ProjectileClass;
};
