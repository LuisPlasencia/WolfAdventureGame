// Made by Luis Plasencia Pulido

#include "AbilitySystem/BaseAbilitySystemGlobals.h"
#include "BaseAbilityTypes.h"


FGameplayEffectContext* UBaseAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBaseGameplayEffectContext();
}
