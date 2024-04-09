// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WolfAdventureGameModeBase.generated.h"


class UCharacterClassInfo;
class UAbilityInfo;
class ULootTiers;
class UMVVM_LoadSlot;
class USaveGame;
class ULoadScreenSaveGame;

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API AWolfAdventureGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
	TObjectPtr<ULootTiers> LootTiers;

	// the game mode is a good class to save, since game modes have authority over the rules of the game 
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;

	void TravelToMap(UMVVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	// the maps will be set in the editor's panel so we can make them soft object pointers with the template wrapper TSoftObjectPtr (not loaded in memory until they are used) (lazily loaded)
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(ACharacter* DeadCharacter);

protected:
	virtual void BeginPlay() override;

};
