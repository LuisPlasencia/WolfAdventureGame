// Made by Luis Plasencia Pulido


#include "UI/WidgetController/BaseWidgetController.h"

void UBaseWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UBaseWidgetController::BroadcastInitialValues()
{

}

void UBaseWidgetController::BindCallbacksToDependencies()
{

}
