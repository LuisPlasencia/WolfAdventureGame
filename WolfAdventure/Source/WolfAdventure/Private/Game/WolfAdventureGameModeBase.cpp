// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WolfAdventureGameModeBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerStart.h"
#include <Game/LoadScreenSaveGame.h>
#include <Game/WolfAdventureGameInstance.h>
#include <EngineUtils.h>
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include <Interaction/SaveInterface.h>

void AWolfAdventureGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* AWolfAdventureGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame =  Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AWolfAdventureGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AWolfAdventureGameModeBase::RetrieveInGameSaveData()
{
	UWolfAdventureGameInstance* BaseGameInstance = Cast<UWolfAdventureGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = BaseGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = BaseGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void AWolfAdventureGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UWolfAdventureGameInstance* BaseGameInstance = Cast<UWolfAdventureGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = BaseGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = BaseGameInstance->LoadSlotIndex;
	BaseGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void AWolfAdventureGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UWolfAdventureGameInstance* BaseGI = Cast<UWolfAdventureGameInstance>(GetGameInstance());
	check(BaseGI);

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(BaseGI->LoadSlotName, BaseGI->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}

		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
		
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty(); // we make a copy and we clear it out, we'll fill it in with "actors" and then replace the one in the array

		// an alternative to iterating all actors in the world would be to iterate over an array of actors that we want to save and add actors to that array each time we change their state (just a little performance upgrade)
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName(); // unique name
			SavedActor.Transform = Actor->GetTransform();

			// Archive for storing arbitrary data to the specified memory location
			FMemoryWriter MemoryWriter(SavedActor.Bytes);

			// Archive designed to serialize an UObject and FNames (an Actor for instance)
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;  // makes the archive compatible with a savegame object 

			// we serialize the actor into the archive (just the variables with SaveGame UPROPERTY specifier)
			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}
		
		// we replace the map in the array
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, BaseGI->LoadSlotName, BaseGI->LoadSlotIndex);
	}
}

void AWolfAdventureGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UWolfAdventureGameInstance* BaseGI = Cast<UWolfAdventureGameInstance>(GetGameInstance());
	check(BaseGI);

	if (UGameplayStatics::DoesSaveGameExist(BaseGI->LoadSlotName, BaseGI->LoadSlotIndex))
	{
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(BaseGI->LoadSlotName, BaseGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load slot"));
			return;
		}

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					// we deserialize the variables that we serialized using the Archives
					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);  // converts binary bytes back into variables (deserialize)

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}

		}
	}
}

void AWolfAdventureGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

FString AWolfAdventureGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

AActor* AWolfAdventureGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UWolfAdventureGameInstance* BaseGameInstance = Cast<UWolfAdventureGameInstance>(GetGameInstance());

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == BaseGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}

			}
		}
		return SelectedActor;
	}
	
	return nullptr;
}

void AWolfAdventureGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}
