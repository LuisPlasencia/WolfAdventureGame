// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "WolfPlayerController.generated.h"


class UInputMappingContext;
class UBaseInputConfig;
class UBaseAbilitySystemComponent;
class UInputAction;
class UDamageTextComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API AWolfPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWolfPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	// CLIENT RPC (for the server controlled server will be executed on the server, for a client controlled character it will be called on the server but executed on the server and the client will see it)
	UFUNCTION(Client, Reliable)
		void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		TObjectPtr<UInputAction> JumpAction;

	virtual void SetupInputComponent() override;

private:
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;
	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

	void CrosshairTrace();
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;
	FHitResult CursorHit;
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UBaseInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UBaseAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	bool isJumping = false;

	UPROPERTY(EditAnywhere)
	float timeBetweenJumps = 1.0f;

	void FinishJumping();
};
