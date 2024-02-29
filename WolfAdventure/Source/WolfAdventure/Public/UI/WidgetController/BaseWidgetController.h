// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseWidgetController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FBaseAbilityInfo&, Info);

class UAbilitySystemComponent;
class UAttributeSet;
class AWolfPlayerController;
class AWolfPlayerState;
class UBaseAbilitySystemComponent;
class UBaseAttributeSet;
class UAbilityInfo;


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class WOLFADVENTURE_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();


	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AWolfPlayerController> WolfPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AWolfPlayerState> WolfPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UBaseAttributeSet> BaseAttributeSet;

	AWolfPlayerController* GetWolfPC();
	AWolfPlayerState* GetWolfPS();
	UBaseAbilitySystemComponent* GetBaseASC();
	UBaseAttributeSet* GetBaseAS();

};
