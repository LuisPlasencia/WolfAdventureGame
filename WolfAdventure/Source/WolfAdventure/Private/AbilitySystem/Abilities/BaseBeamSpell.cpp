// Made by Luis Plasencia Pulido


#include "AbilitySystem/Abilities/BaseBeamSpell.h"

void UBaseBeamSpell::StoreCrosshairDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		CrosshairHitLocation = HitResult.ImpactPoint;
		CrosshairHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UBaseBeamSpell::StoreOwnerPlayerController()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
	
}
