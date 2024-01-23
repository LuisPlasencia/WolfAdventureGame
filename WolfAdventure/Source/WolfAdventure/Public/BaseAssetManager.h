// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BaseAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UBaseAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
