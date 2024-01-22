// Made by Luis Plasencia Pulido


#include "Player/WolfPlayerState.h"
#include <AbilitySystem/BaseAbilitySystemComponent.h>
#include <AbilitySystem/BaseAttributeSet.h>
#include <Net/UnrealNetwork.h>

AWolfPlayerState::AWolfPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

void AWolfPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWolfPlayerState, Level);
}

UAbilitySystemComponent* AWolfPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AWolfPlayerState::OnRep_Level(int32 OldLevel)
{

}
