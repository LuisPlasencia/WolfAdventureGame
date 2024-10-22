// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "WolfPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /* StatValue */, bool /*bLevelUp*/)

/**
 *  this class is replicated (clients & server have this class)
 */

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

UCLASS()
class WOLFADVENTURE_API AWolfPlayerState : public APlayerState , public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AWolfPlayerState();

	// To replicate a variable we need to add it to this method
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnLevelChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;


	// forceInline = whenever we call this function, the preprocessor will replace the function call when preprocessing (before compilation step) with the function body (the Level variable) (the compiler has the say over whether it ends up doing it or not)   (minor performance optimization)
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InAttributePoints);
	void SetSpellPoints(int32 InSpellPoints);

protected: 

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UAbilitySystemComponent>  AbilitySystemComponent;

	UPROPERTY()
		TObjectPtr<UAttributeSet>  AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;


	// Rep notify for the client side
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
		void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
		void OnRep_SpellPoints(int32 OlddSpellPoints);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);
};
