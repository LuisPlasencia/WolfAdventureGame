// Made by Luis Plasencia Pulido


#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include <UI/HUD/BaseHUD.h>
#include <UI/WidgetController/BaseWidgetController.h>
#include <Player/WolfPlayerState.h>

UOverlayWidgetController* UBaseAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(PC->GetHUD()))
		{
			AWolfPlayerState* PS = PC->GetPlayerState<AWolfPlayerState>(); 	// it takes an input template parameter
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return BaseHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UBaseAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(PC->GetHUD()))
		{
			AWolfPlayerState* PS = PC->GetPlayerState<AWolfPlayerState>(); 	// it takes an input template parameter
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return BaseHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
