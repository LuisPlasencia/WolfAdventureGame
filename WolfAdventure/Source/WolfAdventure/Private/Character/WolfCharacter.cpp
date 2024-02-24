#include "Character/WolfCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include <Interaction/EnemyInterface.h>
#include <Player/WolfPlayerState.h>
#include "AbilitySystemComponent.h"
#include <Player/WolfPlayerController.h>
#include <UI/HUD/BaseHUD.h>
#include <AbilitySystem/BaseAbilitySystemComponent.h>

// Sets default values
AWolfCharacter::AWolfCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Elementalist;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BiteBox"));
	BoxComponent->AddRelativeRotation(FRotator(44,50,2));
	BoxComponent->AddRelativeLocation(FVector(92,5,20));
	BoxComponent->SetBoxExtent(FVector(32,32,32));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName("OverlapAllDynamics");

	BoxComponent->SetupAttachment(GetRootComponent());
}

void AWolfCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AWolfCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AWolfCharacter::GetPlayerLevel()
{
	const AWolfPlayerState * WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	return WolfPlayerState->GetPlayerLevel();
}

void AWolfCharacter::InitAbilityActorInfo()
{
	AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState); //assert (crash when null)
	WolfPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(WolfPlayerState, this);
	Cast<UBaseAbilitySystemComponent>(WolfPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = WolfPlayerState->GetAbilitySystemComponent();
	AttributeSet = WolfPlayerState->GetAttributeSet();


	// on the client side, we only have the playercontroller for our character, not the other player's so we need to nullcheck this (server has all the player controllers but not the client)
	if (AWolfPlayerController* WolfPlayerController = Cast<AWolfPlayerController>(GetController()))
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(WolfPlayerController->GetHUD()))   // hud only valid for the locally controlled player
		{
			BaseHUD->InitOverlay(WolfPlayerController, WolfPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// only needs to be done in the server since the attributes will be replicated to the clients (but can be done in both client and server so the client doesnt have to wait for the server to replicate them back down)
	InitializeDefaultAttributes();
}

// Called when the game starts or when spawned
void AWolfCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WolfMappingContext, 0);
		}
	}
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWolfCharacter::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AWolfCharacter::OnEndOverlap);
}

// Called every frame
void AWolfCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWolfCharacter::Move(const FInputActionValue& Value)
{
	// if (ActionState != EActionState::EAS_Unoccupied) return;  we dont wanna move when we dont want to move
	const FVector2D MovementVector = Value.Get<FVector2D>();

	//const FVector Forward = GetActorForwardVector();
	//AddMovementInput(Forward, MovementVector.Y);

	//const FVector Right = GetActorRightVector();
	//AddMovementInput(Right, MovementVector.X);

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void AWolfCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AWolfCharacter::Jump(const FInputActionValue& Value)
{
	if (!isJumping)
	{
		isJumping = true;
		FTimerHandle JumpTimer;
		GetWorldTimerManager().SetTimer(JumpTimer, this, &AWolfCharacter::FinishJumping, timeBetweenJumps, false);
		Cast<ACharacter>(this)->Jump();
	}

}

void AWolfCharacter::Dodge()
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
}

void AWolfCharacter::Attack()
{
	//if (!isAttacking)
	//{
	//	isAttacking = true;
	//	FTimerHandle AttackTimer;
	//	GetWorldTimerManager().SetTimer(AttackTimer, this, &AWolfCharacter::FinishAttacking, timeBetweenAttacks, false);
	//}

	if (ActionState == EActionState::EAS_Unoccupied)
	{
	//	PlayAttackMontage();
	//	ActionState = EActionState::EAS_Attacking;
	}

}

void AWolfCharacter::Equip()
{


}

void AWolfCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AWolfCharacter::PlayDodgeMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage);
	}
}

void AWolfCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AWolfCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AWolfCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IEnemyInterface* ThisActor = Cast<IEnemyInterface>(OtherActor))
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighLightActor();

		}
	//	UE_LOG(LogTemp, Warning, TEXT("BeginOverlapPig"));
	}


}

void AWolfCharacter::OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (IEnemyInterface* ThisActor = Cast<IEnemyInterface>(OtherActor))
	{
		if (OtherActor != nullptr)
		{
			ThisActor->UnHighlightActor();
		}
	//	UE_LOG(LogTemp, Warning, TEXT("EndOverlapPig"));
	}
}


void AWolfCharacter::FinishJumping()
{
	isJumping = false;
}

void AWolfCharacter::FinishAttacking()
{
	isAttacking = false;
}


// Called to bind functionality to input
void AWolfCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Jump);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Dodge);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Equip);
	}

}



