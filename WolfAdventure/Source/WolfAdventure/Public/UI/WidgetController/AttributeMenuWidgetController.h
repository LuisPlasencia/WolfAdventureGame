// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UAttributeMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies();
	virtual void BroadcastInitialValues();

};
