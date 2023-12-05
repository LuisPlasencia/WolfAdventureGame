#include "Character/WolfCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

// Sets default values
AWolfCharacter::AWolfCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
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
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
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

