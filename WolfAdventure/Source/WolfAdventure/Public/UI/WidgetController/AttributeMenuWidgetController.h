// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FBaseAttributeInfo;
struct FGameplayAttribute;
struct FGameplayTag;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FBaseAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WOLFADVENTURE_API UAttributeMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies();
	virtual void BroadcastInitialValues();

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

};
