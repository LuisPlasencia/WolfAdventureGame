// Made by Luis Plasencia Pulido


#include "Player/WolfPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include <Input/BaseInputComponent.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include <BaseGameplayTags.h>
#include <Interaction/EnemyInterface.h>
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AWolfPlayerController::AWolfPlayerController()
{
	bReplicates = true;

}

void AWolfPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CrosshairTrace();
}

void AWolfPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	// is valid checks is pending kill as well, target character may have a destroy call on it recently but damagetextcomponent is just a property that we aither set or not 
	// isLocalController so we can only see the damage text if we are the local controller (if we are on the server and we are calling this on a client owned player controller, it will return false and the server will not see those damage numbers) 
	// Client's RPC's like this function are executed on the server but also on the client, thats why we make IsLocalController check, so that we can only see the damage floating text on the correct machine
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController()) 
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);

		// after we create a widget component dynamically we need to register it, we dont see this that often because we usually construct components with createDefaultSubObject that handles registration for us
		// we are creating a component dynamically not in the constructor so we are manually registering this component
		DamageText->RegisterComponent();  

		// so that it starts off at the correct location and plays the animation
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		// we dont want it to follow the enemy around so we detach it and let it float away according to its own animation
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
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

	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void AWolfPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}

	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);

	// GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AWolfPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}

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
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_CrosshairTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

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
