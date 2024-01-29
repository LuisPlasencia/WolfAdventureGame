// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WOLFADVENTURE_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	// in this case we dont make it pure virtual so the implementation is not compulsary but we can define a default return value in the cpp
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();
};
