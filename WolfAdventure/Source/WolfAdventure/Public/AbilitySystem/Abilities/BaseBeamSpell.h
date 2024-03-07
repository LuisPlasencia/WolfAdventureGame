// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "BaseBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseBeamSpell : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	// for algorithms its best to create them in c++ and expose them to blueprint (+performance)
	// blueprints are nice for logic flow 
	// for computentionally intense algorithms c++ takes the cake

	UFUNCTION(BlueprintCallable)
	void StoreCrosshairDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayerController();

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector CrosshairHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> CrosshairHitActor;

	// UPROPERTies are not weak object pointers! 
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
};
