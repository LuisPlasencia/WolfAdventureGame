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
#include "NiagaraComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include <AbilitySystem/Data/LevelUpInfo.h>
#include <Game/WolfAdventureGameModeBase.h>
#include <Kismet/GameplayStatics.h>
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/LoadScreenSaveGame.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include <Game/WolfAdventureGameInstance.h>
#include <AbilitySystem/BaseAbilitySystemLibrary.h>

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
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->SetUsingAbsoluteRotation(true);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BiteBox"));
	BoxComponent->AddRelativeRotation(FRotator(44,50,2));
	BoxComponent->AddRelativeLocation(FVector(92,5,20));
	BoxComponent->SetBoxExtent(FVector(32,32,32));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName("OverlapAllDynamics");

	BoxComponent->SetupAttachment(GetRootComponent());

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}


void AWolfCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
	LoadProgress();

	if (AWolfAdventureGameModeBase* BaseGameMode = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		BaseGameMode->LoadWorldState(GetWorld());
	}
}

// saving to disk is a single player solution, in multiplayer we would be retrieving the data from a database
void AWolfCharacter::LoadProgress()
{
	AWolfAdventureGameModeBase* BaseGameMode = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (BaseGameMode)
	{
		ULoadScreenSaveGame* SaveData = BaseGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			//Load in Abilities from disk 
			if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
			{
				BaseASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}

			if (AWolfPlayerState* WolfPlayerState = Cast<AWolfPlayerState>(GetPlayerState()))
			{
				WolfPlayerState->SetLevel(SaveData->PlayerLevel);
				WolfPlayerState->SetXP(SaveData->XP);
				WolfPlayerState->SetAttributePoints(SaveData->AttributePoints);
				WolfPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}

			UBaseAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void AWolfCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

void AWolfCharacter::AddToXP_Implementation(int32 InXP)
{
	AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	WolfPlayerState->AddToXP(InXP);
}

void AWolfCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AWolfCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = ViewCamera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AWolfCharacter::GetXP_Implementation() const
{
	const AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	return WolfPlayerState->GetXP();
}

int32 AWolfCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	return WolfPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AWolfCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	return WolfPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AWolfCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	return WolfPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AWolfCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	WolfPlayerState->AddToLevel(InPlayerLevel);

	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		BaseASC->UpdateAbilityStatuses(WolfPlayerState->GetPlayerLevel());
	}
}

void AWolfCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	WolfPlayerState->AddToAttributePoints(InAttributePoints);
}

void AWolfCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	WolfPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AWolfCharacter::GetAttributePoints_Implementation() const
{
	AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	return WolfPlayerState->GetAttributePoints();
}

int32 AWolfCharacter::GetSpellPoints_Implementation() const
{
	AWolfPlayerState* WolfPlayerState = GetPlayerState<AWolfPlayerState>();
	check(WolfPlayerState);
	return WolfPlayerState->GetSpellPoints();
}

void AWolfCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	// this class depends on the player controller, not the other way around (one way dependency) (+avoid cicling dependencies)
	if (AWolfPlayerController* WolfPlayerController = Cast<AWolfPlayerController>(GetController()))
	{
		WolfPlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void AWolfCharacter::HideMagicCircle_Implementation()
{
	if (AWolfPlayerController* WolfPlayerController = Cast<AWolfPlayerController>(GetController()))
	{
		WolfPlayerController->HideMagicCircle();
	}
}

void AWolfCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AWolfAdventureGameModeBase* BaseGameMode = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (BaseGameMode)
	{
		ULoadScreenSaveGame* SaveData = BaseGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckpointTag;

		if (AWolfPlayerState* WolfPlayerState = Cast<AWolfPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = WolfPlayerState->GetPlayerLevel();
			SaveData->XP = WolfPlayerState->GetXP();
			SaveData->AttributePoints = WolfPlayerState->GetAttributePoints();
			SaveData->SpellPoints = WolfPlayerState->GetSpellPoints();
		}
		SaveData->Strength = UBaseAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UBaseAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UBaseAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UBaseAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		SaveData->bFirstTimeLoadIn = false;

		if (!HasAuthority()) return;
		
		UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, BaseASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = BaseASC->GetAbilityTagFromSpec(AbilitySpec);
			// get abilityinfo will only return ability info on the server because it gets it from the game mode
			UAbilityInfo* AbilityInfo = UBaseAbilitySystemLibrary::GetAbilityInfo(this);
			FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbility = Info.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = BaseASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = BaseASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;

			// AddUnique must be able to compare two savedAbilities to know if they are equal or not (we need to override equal "==" operator)
			SaveData->SavedAbilities.AddUnique(SavedAbility);
		});
		BaseASC->ForEachAbility(SaveAbilityDelegate);

		BaseGameMode->SaveInGameProgressData(SaveData);
	}
}

int32 AWolfCharacter::GetPlayerLevel_Implementation()
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

	// we could call this in super since both the enemies and the player call it in the same method but that is prone to error and fragile since we would have to call super after setting the ASC, not before
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AWolfCharacter::StunTagChanged);


	// on the client side, we only have the playercontroller for our character, not the other player's so we need to nullcheck this (server has all the player controllers but not the client)
	if (AWolfPlayerController* WolfPlayerController = Cast<AWolfPlayerController>(GetController()))
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(WolfPlayerController->GetHUD()))   // hud only valid for the locally controlled player
		{
			BaseHUD->InitOverlay(WolfPlayerController, WolfPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// only needs to be done in the server since the attributes will be replicated to the clients (but can be done in both client and server so the client doesnt have to wait for the server to replicate them back down)
	//	InitializeDefaultAttributes();
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

//client
void AWolfCharacter::OnRep_Stunned()
{
	// we add or remove blocked tags on the clients with this rep notify because in baseattributeset Debuff() function, they are only added on the server since the function only runs on the server
	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_CrosshairTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			BaseASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			BaseASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AWolfCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

// Called every frame
void AWolfCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWolfCharacter::Move(const FInputActionValue& Value)
{
	/*
	
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


	*/


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
	/*
	
	if (!isJumping)
	{
		isJumping = true;
		FTimerHandle JumpTimer;
		GetWorldTimerManager().SetTimer(JumpTimer, this, &AWolfCharacter::FinishJumping, timeBetweenJumps, false);
		Cast<ACharacter>(this)->Jump();
	}
	*/
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
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Look);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Jump);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Dodge);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AWolfCharacter::Equip);
	}

}



