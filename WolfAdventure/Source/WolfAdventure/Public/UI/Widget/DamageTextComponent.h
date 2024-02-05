// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damagebool, bool bBlockedHit, bool bCriticalHit);

};
