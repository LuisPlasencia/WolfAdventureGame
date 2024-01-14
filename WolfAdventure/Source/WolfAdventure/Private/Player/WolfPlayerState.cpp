// Made by Luis Plasencia Pulido


#include "Player/WolfPlayerState.h"
#include <AbilitySystem/BaseAbilitySystemComponent.h>
#include <AbilitySystem/BaseAttributeSet.h>

AWolfPlayerState::AWolfPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AWolfPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
