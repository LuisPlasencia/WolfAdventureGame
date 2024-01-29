// Made by Luis Plasencia Pulido


#include "AbilitySystem/AbilityTasks/TargetDataUnderCrosshair.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderCrosshair* UTargetDataUnderCrosshair::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCrosshair* MyObj = NewAbilityTask<UTargetDataUnderCrosshair>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderCrosshair::Activate()
{
	// if this is not a locally controlled player, cursorhit will not have valid data (we need to send data to the server)
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendCrosshairData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		// We are on the server, so listen for target data (we bind our callback to the delegate)
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderCrosshair::OnTargetDataReplicatedCallback);
		
		// if the delegate has already received targetData we call it (it makes the broadcast again) (if targetData has already been sent and the delegate has already been broadcast)
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);

		// if it hasnt reached the server yet, we'll be waiting for that data to come back
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}


}

// called on the client
void UTargetDataUnderCrosshair::SendCrosshairData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	FHitResult CrosshairHit;

	// Viewport Size
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	FVector2D CrosshairPosition = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 3);
	PC->GetHitResultAtScreenPosition(CrosshairPosition, ECC_WorldDynamic, false, CrosshairHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CrosshairHit;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);


	// only broadcasts the delegate if the ability is still active (client side)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

// called on the server
void UTargetDataUnderCrosshair::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	//we make sure the ability system component knows the data has been received so it stops storing (dont need to cache it anymore) replicated target data in the data structure (replication in GAS can be from client to server)
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// only broadcasts the delegate if the ability is still active (server side)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}

}
