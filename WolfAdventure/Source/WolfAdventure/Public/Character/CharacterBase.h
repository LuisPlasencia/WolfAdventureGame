// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"


UCLASS(Abstract)  //prevents the class to be dragged into the level
class WOLFADVENTURE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

};
