// Made by Luis Plasencia Pulido


#include "BaseAssetManager.h"
#include "BaseGameplayTags.h"

UBaseAssetManager& UBaseAssetManager::Get()
{
    check(GEngine)
    UBaseAssetManager* BaseAssetManager = Cast<UBaseAssetManager>(GEngine->AssetManager);
    return *BaseAssetManager;
}

void UBaseAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    FBaseGameplayTags::InitializeNativeGameplayTags();
}
