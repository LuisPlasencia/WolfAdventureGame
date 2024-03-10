// Made by Luis Plasencia Pulido


#include "AbilitySystem/Abilities/BasePassiveAbility.h"
#include <AbilitySystem/BaseAbilitySystemComponent.h>
#include "AbilitySystemBlueprintLibrary.h"

void UBasePassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		BaseASC->DeactivatePassiveAbility.AddUObject(this, &UBasePassiveAbility::ReceiveDeactivate);
	}

}

void UBasePassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	// we check the tags that this ability has
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
