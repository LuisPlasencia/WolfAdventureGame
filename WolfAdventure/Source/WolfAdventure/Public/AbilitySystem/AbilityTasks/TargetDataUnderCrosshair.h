// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCrosshair.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrosshairTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UTargetDataUnderCrosshair : public UAbilityTask
{
	GENERATED_BODY()

public:

	// we default owning ability to self, BlueprintInternalUseOnly = This function is an internal implementation detail, used to implement another function or node. It is never directly exposed in a Blueprint graph. (async node / latent)
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderCrosshair", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "True"))
		static UTargetDataUnderCrosshair* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
		FCrosshairTargetDataSignature ValidData;

private:

	virtual void Activate() override;

	// called on the client
	void SendCrosshairData();

	// called on the server
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
