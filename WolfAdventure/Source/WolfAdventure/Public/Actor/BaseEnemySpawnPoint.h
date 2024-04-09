// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "BaseEnemySpawnPoint.generated.h"

class AEnemyCharacter;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API ABaseEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	TSubclassOf<class AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

};
