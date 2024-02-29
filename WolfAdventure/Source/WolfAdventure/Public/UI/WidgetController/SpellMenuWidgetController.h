// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WOLFADVENTURE_API USpellMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()


public:

	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;
};
