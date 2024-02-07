// Made by Luis Plasencia Pulido


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include <Kismet/GameplayStatics.h>
#include <BehaviorTree/BTFunctionLibrary.h>

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();

	// if we check for its tag (set in the details panel) we dont have to cast (+ efficiency)
	// our target tag will be set to enemy if the owner of this service is the player (we can have a wolf companion that is friendly with a Player tag)
	// our target tag will be set to player if the owner of this service is the enemy
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	// by checking all player actors, we make this compatible to multiplayer
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for(AActor * Actor : ActorsWithTag)
	{
		GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Orange, *Actor->GetName());

		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
