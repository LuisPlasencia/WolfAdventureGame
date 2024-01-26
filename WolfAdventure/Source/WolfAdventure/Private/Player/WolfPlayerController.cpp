// Made by Luis Plasencia Pulido


#include "Player/WolfPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include <Input/BaseInputComponent.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include <BaseGameplayTags.h>

AWolfPlayerController::AWolfPlayerController()
{
	bReplicates = true;
}

void AWolfPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(InputComponent);

	BaseInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);;
}

void AWolfPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AWolfPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);

	// GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AWolfPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// casting every frame is expensive!! we store the cast and just do it once
	// because asc can be null if we call it too early in the game we return early
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);

	//if (!InputTag.MatchesTagExact(FBaseGameplayTags::Get().InputTag_LMB))
	//{
	//	if (GetASC())
	//	{
	//		GetASC()->AbilityInputTagReleased(InputTag);
	//	}
	//}

	// GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
}

UBaseAbilitySystemComponent* AWolfPlayerController::GetASC()
{
	if (BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));

	}
	return BaseAbilitySystemComponent;
}



