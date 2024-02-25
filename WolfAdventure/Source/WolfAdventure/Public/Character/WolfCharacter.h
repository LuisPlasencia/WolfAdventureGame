#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterType.h"
#include "Interaction/PlayerInterface.h"
#include "WolfCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UBoxComponent;

UCLASS()
class WOLFADVENTURE_API AWolfCharacter : public ACharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWolfCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Player interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	/** end Player interface */


	/** Combat interface */
	virtual int32 GetPlayerLevel() override;
	/** end combat interface */

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> WolfMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere)
		float timeBetweenJumps = 1.0f;

	UPROPERTY(EditAnywhere)
		float timeBetweenAttacks = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAttacking = false;

	bool isJumping = false;

	void FinishJumping();
	void FinishAttacking();


	// We cant forward declase FInputActionValue since it is not a pointer, but a Fstruct 
	/**
	* Callbacks for input
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Dodge();
	void Attack();
	void Equip();


	/**
	* Play montage functions
	*/
	void PlayAttackMontage();

	void PlayDodgeMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	/** 
	* Animation montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;


	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void InitAbilityActorInfo() override;
};
