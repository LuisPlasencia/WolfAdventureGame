// Made by Luis Plasencia Pulido


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include <AbilitySystem/BaseAttributeSet.h>
#include "AbilitySystem/Data/AttributeInfo.h"
#include <Player/WolfPlayerState.h>
#include <AbilitySystem/BaseAbilitySystemComponent.h>

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);

	for (auto& Pair : GetBaseAS()->TagsToAttributes)
	{
		// we dont wanna capture Pair by reference because by the time the attribute changes and the delegate gets broadcast the Pair variable (local in for loop) would have gone out of scope so we capture by value instead (copy stored)
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}

		);
	}

	GetWolfPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);

}

// this widget is created each time we press the attribute button and it needs to know its initial values based on the player state or the attribute set and so on
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UBaseAttributeSet* AS = CastChecked<UBaseAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		// doesnt need to know what attributes there are so it is cleaner than the even less ideal way (more generic and automated widget controller)
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	AttributePointsChangedDelegate.Broadcast(GetWolfPS()->GetAttributePoints());


	// Less ideal way
	//for (auto& Pair : AS->TagsToAttributes)
	//{
	//	// doesnt need to know what attributes there are so it is cleaner than the even less ideal way (more generic and automated widget controller)
	//	FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
	//	Info.AttributeValue = Pair.Value.Execute().GetNumericValue(AS);  // executes the function bound to the delegate
	//	AttributeInfoDelegate.Broadcast(Info);
	//}

	// Even Less ideal way:
	//FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FBaseGameplayTags::Get().Attributes_Primary_Strength);
	//Info.AttributeValue = AS->GetStrength();
	//AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UBaseAbilitySystemComponent* BaseASC = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	BaseASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FBaseAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);  // executes the function pointer directly (no need for delegate, no need for execute, + cleaner)
	AttributeInfoDelegate.Broadcast(Info);
}
