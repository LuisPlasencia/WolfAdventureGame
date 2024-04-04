// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WolfAdventureGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UWolfAdventureGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	// WE WANT these variables to participate in the Garbage collection system, hense we add the UPROPERTY macro
	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()	
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;

};
