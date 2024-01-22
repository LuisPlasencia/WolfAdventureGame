// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "WolfPlayerState.generated.h"

/**
 * 
 */

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class WOLFADVENTURE_API AWolfPlayerState : public APlayerState , public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AWolfPlayerState();

	// To replicate a variable we need to add it to this method
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }


	// forceInline = whenever we call this function, the preprocessor will replace the function call when preprocessing (before compilation step) with the function body (the Level variable) (the compiler has the say over whether it ends up doing it or not)   (minor performance optimization)
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

protected: 

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UAbilitySystemComponent>  AbilitySystemComponent;

	UPROPERTY()
		TObjectPtr<UAttributeSet>  AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
