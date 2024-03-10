// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ABaseAIController;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	// at this point we can savely cast the controller 
	virtual void PossessedBy(AController* NewController) override;


	/** Enemy Interface */
	virtual void HighLightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** end Combat interface */

	// this delegate already exists in overlaywidgetcontroller so we can reuse it at the cost of including the header class (not huge so it's ok) (an alternative is moving the delegate to a common class)
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// we dont replicate the level on the enemy because we only check the level on the server on AI controlled enemies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")  // we dont use blueprintreadonly in private section
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABaseAIController> BaseAIController;
};
