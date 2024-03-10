// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
// we want to be able to bind to BP since it is really useful to have a delegate for when something dies (hence dynamic)
// remember that the enemies dies and then x seconds later they disappear and get destroyed, so it is useful to know when they are in the "dead" state which is different from the destroyed one
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);


USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;
class USystem;

/**
 * 
 */
class WOLFADVENTURE_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	// in this case we dont make it pure virtual so the implementation is not compulsary but we can define a default return value in the cpp
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	// for blueprint imeplementable events we do not make them virtual
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	
	// we want to override the function with virtual but we want to call it from blueprint as well
	// we cant use blueprintcallable since we want to use virtual and override it
	// we use blueprint native event so we dont have to mark it as virtual, it will automatically generate a virtual native version in C++ that we can override (version with implementation at the end)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die(const FVector& DeathImpulse) = 0;
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	// if we are casting inside it, we cant make it const. We want to pass "this" in CharacterBaseActor and that is not an Actor but a child so an implicit Cast will make the function non constable
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool bInLoop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShocked() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInShock);
};
