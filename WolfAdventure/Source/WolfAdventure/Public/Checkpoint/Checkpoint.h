// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Interaction/HighlightInterface.h"
#include <WolfAdventure/WolfAdventure.h>
#include "Checkpoint.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	/* Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation();
	/* end Save Interface */

	// SaveGame specifier makes the variable serializable in order for us to be able to save its value to disk (SaveWorldState method in the gamemodde)
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	// false if we want to implement a different overlap mechanic in blueprint for different set of checkpoint items (for example a beacon that lights up but doesnt save the game but we want it to be a checkpoint actor because it does something similar)
	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;

protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/* end Highlight Interface */

	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
};
