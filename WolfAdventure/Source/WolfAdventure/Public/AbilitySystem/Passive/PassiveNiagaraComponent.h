// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"

class UBaseAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	virtual void BeginPlay() override;

	// callback
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
	void ActivateIfEquipped(UBaseAbilitySystemComponent* BaseASC);

};
