// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);  // dynamic multicast delegate that broadcasts a float (delegate type)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WOLFADVENTURE_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")  // we can bind to this delegate on blueprint
	FOnHealthChangedSignature OnHealthChanged;  // member variable (delegate)

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;


	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;


	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;


protected: 
	void HealthChanged(const FOnAttributeChangeData& Data) const; // needs this signature (return + params) in order to be binded as a callback to the delegates in the ASC
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const; 
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
};
