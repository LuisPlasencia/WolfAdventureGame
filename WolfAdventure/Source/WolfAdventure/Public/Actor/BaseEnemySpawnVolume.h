// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "BaseEnemySpawnVolume.generated.h"

class UBoxComponent;
class ABaseEnemySpawnPoint;

UCLASS()
class WOLFADVENTURE_API ABaseEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ABaseEnemySpawnVolume();

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// we want to set this from each individual instance in the world
	UPROPERTY(EditAnywhere)
	TArray<ABaseEnemySpawnPoint*> SpawnPoints;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;


};
