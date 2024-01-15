// Made by Luis Plasencia Pulido


#include "AbilitySystem/BaseAttributeSet.h"
#include <Net/UnrealNetwork.h>

UBaseAttributeSet::UBaseAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
}

//with this function we register variables for replication (boiler plate steps for GAS (Gameplay Ability System))
void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);     // we notify even though it may be the same since sometimeS we may want to respond to the action of setting the variable (REPNOTIFY_ALWAYS)
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);   // inform ability system of the change (MACRO)
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Mana, OldMana);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
}
