// Made by Luis Plasencia Pulido


#include "Player/WolfPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include <Input/BaseInputComponent.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include <BaseGameplayTags.h>
#include <Interaction/EnemyInterface.h>

AWolfPlayerController::AWolfPlayerController()
{
	bReplicates = true;

}

void AWolfPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CrosshairTrace();
}

void AWolfPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(InputComponent);

	BaseInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AWolfPlayerController::ShiftPressed);
	BaseInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AWolfPlayerController::ShiftReleased);
	BaseInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);;

}

void AWolfPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AWolfPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);

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


void AWolfPlayerController::HighlightActor(AActor* InActor)
{
	if (IEnemyInterface * ThisEnemy = Cast<IEnemyInterface>(InActor))
	{
		if (ThisEnemy != nullptr)
		{
			ThisEnemy->HighLightActor();
		}
	}
}

void AWolfPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IEnemyInterface* ThisEnemy = Cast<IEnemyInterface>(InActor))
	{
		if (ThisEnemy != nullptr)
		{
			ThisEnemy->UnHighlightActor();
		}
	}
}

void AWolfPlayerController::CrosshairTrace()
{
	FHitResult CrosshairHit;

	// Viewport Size
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	FVector2D CrosshairPosition = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 3);
	GetHitResultAtScreenPosition(CrosshairPosition, ECC_WorldDynamic, false, CrosshairHit);

	if (!CrosshairHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CrosshairHit.GetActor()) && Cast<IEnemyInterface>(CrosshairHit.GetActor()))
	{
		ThisActor = CrosshairHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}
