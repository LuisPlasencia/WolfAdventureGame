// Made by Luis Plasencia Pulido


#include "Checkpoint/MapEntrance.h"
#include <Interaction/PlayerInterface.h>
#include <Game/WolfAdventureGameModeBase.h>
#include <Kismet/GameplayStatics.h>

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a Map Entrance
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (AWolfAdventureGameModeBase* BaseGM = Cast<AWolfAdventureGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// Save the World state as well as the map we are traveling to
			BaseGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
