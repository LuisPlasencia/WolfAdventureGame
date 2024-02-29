// Made by Luis Plasencia Pulido


#include "UI/WidgetController/BaseWidgetController.h"
#include "Player/WolfPlayerController.h"
#include "Player/WolfPlayerState.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include <AbilitySystem/Data/AbilityInfo.h>

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

void UBaseWidgetController::BroadcastAbilityInfo()
{
	// Get Information about all given abilities, look up their Ability Info, and broadcast it to widgets
	if (!GetBaseASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			// we need a way to figure out the ability tag for a given ability spec
			FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(BaseAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			// the input tag is set by code, not in the blueprint data asset, thats why we need to retrieve it
			Info.InputTag = BaseAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = BaseAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
	GetBaseASC()->ForEachAbility(BroadcastDelegate);
}

AWolfPlayerController* UBaseWidgetController::GetWolfPC()
{
	// lazilly cast (only cast if needed)
	if (WolfPlayerController == nullptr)
	{
		WolfPlayerController = Cast<AWolfPlayerController>(PlayerController);
	}
	return WolfPlayerController;
}

AWolfPlayerState* UBaseWidgetController::GetWolfPS()
{
	if (WolfPlayerState == nullptr)
	{
		WolfPlayerState = Cast<AWolfPlayerState>(PlayerState);
	}
	return WolfPlayerState;
}

UBaseAbilitySystemComponent* UBaseWidgetController::GetBaseASC()
{
	if (BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	}
	return BaseAbilitySystemComponent;
}

UBaseAttributeSet* UBaseWidgetController::GetBaseAS()
{
	if (BaseAttributeSet == nullptr)
	{
		BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	}
	return BaseAttributeSet;
}
