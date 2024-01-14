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

protected:
	virtual void BeginPlay() override;

};
