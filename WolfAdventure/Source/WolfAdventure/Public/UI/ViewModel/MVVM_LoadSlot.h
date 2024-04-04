// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;

	void InitializeSlot();

	UPROPERTY()
	int32 SlotIndex;

	// TO USE THIS ENUM WE INCLUDE LOADSCREENSAVEGAME
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	/** Field Notifies **/

	void SetPlayerName(FString InPlayerName);
	void SetMapName(FString InMapName);
	void SetLoadSlotName(FString InLoadSlotName);

	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	FString GetLoadSlotName() const { return LoadSlotName; }


private:

	// we bind variables to the viewmodel with field notify. The setter and getter functions need to contain the fieldnames in their names.
	// blueprintreadwrite cant be private by  default (allows us to make the variable private in c++ but exposed to blueprint, since we want the setter-getter functions to be used and to avoid direct access)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true")) 
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
	
};
