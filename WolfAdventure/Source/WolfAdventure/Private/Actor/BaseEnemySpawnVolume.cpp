// Made by Luis Plasencia Pulido


#include "Actor/BaseEnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Actor/BaseEnemySpawnPoint.h"
#include <Interaction/PlayerInterface.h>

ABaseEnemySpawnVolume::ABaseEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void ABaseEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

void ABaseEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemySpawnVolume::OnBoxOverlap);
	
}

void ABaseEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;

	bReached = true;
	for (ABaseEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



