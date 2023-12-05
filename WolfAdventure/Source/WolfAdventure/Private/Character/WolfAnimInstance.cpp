// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WolfAnimInstance.h"
#include "Character/WolfCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UWolfAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	WolfCharacter = Cast<AWolfCharacter>(TryGetPawnOwner());
	if (WolfCharacter)
	{
		WolfCharacterMovement = WolfCharacter->GetCharacterMovement();
	}
	
}

void UWolfAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (WolfCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(WolfCharacterMovement->Velocity);
		IsFalling = WolfCharacterMovement->IsFalling();
	}
}

