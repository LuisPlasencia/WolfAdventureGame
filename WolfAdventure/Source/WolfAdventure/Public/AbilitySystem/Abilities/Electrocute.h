// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UElectrocute : public UBaseBeamSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
