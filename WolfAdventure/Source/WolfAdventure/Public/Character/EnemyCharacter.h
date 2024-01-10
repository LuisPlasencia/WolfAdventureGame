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
	virtual void HighLightActor() override;
	virtual void UnHighlightActor() override;


};
