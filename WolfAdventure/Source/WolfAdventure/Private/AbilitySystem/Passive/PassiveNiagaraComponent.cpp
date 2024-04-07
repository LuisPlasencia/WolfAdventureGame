// Made by Luis Plasencia Pulido


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include <AbilitySystem/BaseAbilitySystemComponent.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include <Interaction/CombatInterface.h>

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		// we bind the callback to the delegate
		BaseASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		// to avoid a race condition where a broadcast may have been done before this beginplay function is called, if the ability has already been equipped, we activate it
		ActivateIfEquipped(BaseASC);
	}
	// if the ASC is not valid yet, we register to a delegate that will broadcast when the ASC becomes valid
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
			{
				if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
				{
					// we bind the callback to the delegate
					BaseASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
					ActivateIfEquipped(BaseASC);
				} 
			});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UBaseAbilitySystemComponent* BaseASC)
{
	const bool bStartupAbilitiesGiven = BaseASC->bStartupAbilitiesGiven;
	if (bStartupAbilitiesGiven)
	{
		if (BaseASC->GetStatusFromAbilityTag(PassiveSpellTag) == FBaseGameplayTags::Get().Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
