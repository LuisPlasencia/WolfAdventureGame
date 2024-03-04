// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseProjectileSpell.h"
#include "BaseFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseFireBolt : public UBaseProjectileSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;
};
