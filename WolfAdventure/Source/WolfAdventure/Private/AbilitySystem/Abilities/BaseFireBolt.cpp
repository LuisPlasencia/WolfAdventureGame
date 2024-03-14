// Made by Luis Plasencia Pulido


#include "AbilitySystem/Abilities/BaseFireBolt.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Actor/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <AbilitySystem/BaseAbilitySystemLibrary.h>

FString UBaseFireBolt::GetDescription(int32 Level)
{
//	const int32 Damage = GetDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		// %.1f = one decimal to the right and as many as we need to the left
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing </>"

			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"), 

			// Values
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing </>"

			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
	}
}

FString UBaseFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL: </>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Number of FireBolts
		"<Default>Launches %d bolts of fire, "
		"exploding on impact and dealing: </>"

		// Damage
		"<Damage>%d</><Default> fire damage with"
		" a chance to burn</>"),

		// Values
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		ScaledDamage);

}

void UBaseFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	// we want to spawn the projectile on the server and let it handle its movement. The client will see a replicated version of the projectile
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// we want a socket location from an actor that implements the combat interface, this way we can use this in different actors and not just the wolf (abstractions > specifics)
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	// Rotation.Pitch = 0.f; // ground parallel
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward = Rotation.Vector();

	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotations = UBaseAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		// we want to make sure that the properties of the actor projectile are set before spawning it (need of gameplay effect, etc), thats why we use spawndeferred
		ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);  // always spawn regardless of collisions/overrides

		Projectile->SetOwner(GetAvatarActorFromActorInfo());
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			// the homing target component is a weak object pointer because the projectile movement component doesnt want to affect the reference counting 
			//for garbage collection if it´s holding on to something (we dont want the object to stay alive just because our projectile movement component´s 
			//homing target component points to it) We need to be aware that things can become null under a weak object pointer reference  
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			// we need a scene component in the location where the hit has occurred so that we can pass the scene component of the exact point to the homing target component
			// keep in mind! homing target component is a weak pointer, so when the projectile dies and homing target component goes out of scope, the new object scene component 
			// that we are creating is not going to be cleaned up by GC unless we make it a member variable first.
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;

		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		Projectile->FinishSpawning(SpawnTransform);
	}




	/*
	
	//DEBUGGING LINES
	
	TArray<FVector> Directions = UBaseAbilitySystemLibrary::EvenlyRotatedVectors(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (FVector& Direction : Directions)
	{
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			SocketLocation,
			SocketLocation + Direction * 75.f,
			1,
			FLinearColor::Red,
			120,
			1);
	}

	TArray<FRotator> Rotations = UBaseAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (FRotator& Rot : Rotations)
	{
		FVector Start = SocketLocation + FVector(0, 0, 5);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Rot.Vector() * 75.f,
			1,
			FLinearColor::Blue,
			120,
			1);
	}

	*/
}
