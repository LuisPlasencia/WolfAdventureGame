// Made by Luis Plasencia Pulido


#include "UI/WidgetController/OverlayWidgetController.h"
#include <AbilitySystem/BaseAttributeSet.h>

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);  // unreal checks for us (abort program if null)

	OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(BaseAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(BaseAttributeSet->GetMaxMana());

}


void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged); // adduobject and not adddynamic because its not a dynamic delegate (we bind the callback function to the delegate)

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
