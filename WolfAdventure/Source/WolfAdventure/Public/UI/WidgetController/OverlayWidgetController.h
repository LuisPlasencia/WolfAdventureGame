// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"


// the data table that we use to store information depending on the gameplay tag will have these rows (we fill it creating a blueprint data table whose rows have this signature)
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UBaseUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};


// dynamic delegates can be serialized, theyre the slowest delegates, only use when needed (and we need them here in order to be assignable in blueprint)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);  // dynamic multicast delegate that broadcasts a float (delegate type)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FBaseAbilityInfo&, Info);



struct FOnAttributeChangeData;
class UBaseUserWidget;
class UAbilityInfo;
class UBaseAbilitySystemComponent;

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
	FOnAttributeChangedSignature OnHealthChanged;  // member variable (delegate)

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

protected: 

	// data table to store information depending on the gameplay tag 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// We do this with lambdas instead of callback functions now since they just do simple stuff
	//void HealthChanged(const FOnAttributeChangeData& Data) const; // needs this signature (return + params) in order to be binded as a callback to the delegates in the ASC
	//void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	//void ManaChanged(const FOnAttributeChangeData& Data) const; 
	//void MaxManaChanged(const FOnAttributeChangeData& Data) const;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag); // able to return any type of data table row (T is the Type in the template function)

	void OnInitializeStartupAbilities(UBaseAbilitySystemComponent* BaseAbilitySystemComponent);

	void OnXPChanged(int32 NewXP) const;
};


// this is worthy of being in a static function library since it is more versatile than this class needs
template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	// our row is named as the tag in the blueprint data table
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
