// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightInterface.generated.h"

// This class does not need to be modified.
// with blueprint type we can implement functionality for hightlightactor and unhighlightactor in our blueprints
// (easier to extend functionality to different classes since we dont have to create c++ classes for those actors
UINTERFACE(MinimalAPI, BlueprintType)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WOLFADVENTURE_API IHighlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void HighlightActor();

	UFUNCTION(BlueprintNativeEvent)
	void UnHighlightActor();

};
