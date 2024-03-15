// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "BaseFireBlast.generated.h"

class ABaseFireBall;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseFireBlast : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<ABaseFireBall*> SpawnFireBalls();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseFireBall> FireBallClass;
	
};
