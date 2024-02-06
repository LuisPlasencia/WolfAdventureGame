// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "BaseProjectileSpell.generated.h"

class ABaseProjectile;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseProjectileSpell : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category= "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseProjectile> ProjectileClass;

};
