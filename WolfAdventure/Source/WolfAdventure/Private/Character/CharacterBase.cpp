// Made by Luis Plasencia Pulido


#include "Character/CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;


	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

