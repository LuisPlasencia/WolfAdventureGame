// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include <GameplayTagContainer.h>
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

/**
 * 
 */
// blueprinttype is there by default, exposedAsyncProxy is there to have an output pin in the BP node with a reference to the task (class) that we just created so that we can end it if it already exists
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class WOLFADVENTURE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;


	// BlueprintInternalUseOnly is a flag that allows a function to be called from blueprint in a very specific way (only in the event graph, not in a function) and not by people editing the blueprint. "Latent" node (indicated by the little watch icon in the upper right) 
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	// so it will be garbage collected or not depending on the lifetime of the owner
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};
