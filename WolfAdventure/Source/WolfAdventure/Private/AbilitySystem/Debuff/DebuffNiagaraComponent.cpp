// Made by Luis Plasencia Pulido


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include <Interaction/CombatInterface.h>

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	// the ASC may not have been initialized yet in beginplay
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	// our character base is dependant on debuffniagaracomponent, which is a good reason to not let debuffniagaracomponent be dependant on characterbase (we dont want circular dependencies). Thats why we use an interface
	else if (CombatInterface)
	{
		// standard lambdas hold references to the things they capture in their capture list, for example, a niagara component.
		// If that niagara component is garbage collected automatically (with UPROPERTY for example), as long as the lambda holds a reference to it,
		// it will not be automatically deleted.
		// However, a weak lambda has the property of holding a reference to something without increasing its reference count, so it can still be garbage collected.
		// It is as if the weak lambda doesnt have a reference to it at all. "this" will not have its reference count changed 
		// Why do we make this weak? we are just interested in calling this once in begin play and just in case the asc hasnt been created yet, so we dont really want it to make the class linger around more than it should
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC) 
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}
	// if we want to addDynamic instead of addUObject it needs to be a UFUNCTION
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());

	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

