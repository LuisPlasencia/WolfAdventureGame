// Made by Luis Plasencia Pulido


#include "UI/WidgetController/OverlayWidgetController.h"
#include <AbilitySystem/BaseAttributeSet.h>
#include <AbilitySystem/BaseAbilitySystemComponent.h>
#include <AbilitySystem/Data/AbilityInfo.h>

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

	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	//	BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged); // adduobject and not adddynamic because its not a dynamic delegate (we bind the callback function to the delegate)
	//
	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	//	BaseAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	//	BaseAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	//	BaseAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BaseAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
	);

	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{
		// abilities may have been given prior to us binding to the callback (race condition), so we check for that case
		if (BaseASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(BaseASC);
		}
		else
		{
			BaseASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}


		// Binding a lambda (anonymous function) saves us the trouble of declaring callback member functions for all the delegates that we want to bind to (+simplicity)
		BaseASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)  // a reference so we dont copy the tag (+efficiency)
				{
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message")); // returns tags in our config file (project settings)
					if (Tag.MatchesTag(MessageTag))
					{
						//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
						//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

						// in order to call a member function in a lambda we need to capture it since a lambda (anonymous function) doesnt know about the class it is in but does know a function like GEngine since it is global to the project
						// by capturing "this" we are capturing the entire object that we are in (the entire class)
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);

						// we want to send the row to the widget (broadcast it up to the widget)
						// we want a delegate that can send through an FUIWidgetRow
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}

}

void UOverlayWidgetController::OnInitializeStartupAbilities(UBaseAbilitySystemComponent* BaseAbilitySystemComponent)
{
	// Get Information about all given abilities, look up their Ability Info, and broadcast it to widgets
	if (!BaseAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, BaseAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		// we need a way to figure out the ability tag for a given ability spec
		FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(BaseAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		// the input tag is set by code, not in the blueprint data asset, thats why we need to retrieve it
		Info.InputTag = BaseAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	BaseAbilitySystemComponent->ForEachAbility(BroadcastDelegate);

}

//void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
//{
//	OnHealthChanged.Broadcast(Data.NewValue);
//}
//
//void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
//{
//	OnMaxHealthChanged.Broadcast(Data.NewValue);
//}
//
//void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
//{
//	OnManaChanged.Broadcast(Data.NewValue);
//}
//
//void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
//{
//	OnMaxManaChanged.Broadcast(Data.NewValue);
//}
