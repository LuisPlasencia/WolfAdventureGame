// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

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

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	// we dont replicate the level on the enemy because we only check the level on the server on AI controlled enemies
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")  // we dont use blueprintreadonly in private section
	int32 Level = 1;


};
