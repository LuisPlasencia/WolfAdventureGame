// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;

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

	// for blueprint imeplementable events we do not make them virtual
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	
	// we want to override the function with virtual but we want to call it from blueprint as well
	// we cant use blueprintcallable since we want to use virtual and override it
	// we use blueprint native event so we dont have to mark it as virtual, it will automatically generate a virtual native version in C++ that we can override (version with implementation at the end)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;
};
