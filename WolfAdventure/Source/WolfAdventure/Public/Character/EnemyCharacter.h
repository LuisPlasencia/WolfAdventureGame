// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();


	/** Enemy Interface */
	virtual void HighLightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combate Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat interface */

	// this delegate already exists in overlaywidgetcontroller so we can reuse it at the cost of including the header class (not huge so it's ok) (an alternative is moving the delegate to a common class)
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	// we dont replicate the level on the enemy because we only check the level on the server on AI controlled enemies
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")  // we dont use blueprintreadonly in private section
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

};
