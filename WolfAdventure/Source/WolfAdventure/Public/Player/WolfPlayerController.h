// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "WolfPlayerController.generated.h"


class UInputMappingContext;
class UBaseInputConfig;
class UBaseAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API AWolfPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWolfPlayerController();

protected:
	virtual void SetupInputComponent() override;

private:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UBaseInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UBaseAbilitySystemComponent* GetASC();
};
