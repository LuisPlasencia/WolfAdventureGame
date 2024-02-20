// Made by Luis Plasencia Pulido


#include "AbilitySystem/Abilities/BaseProjectileSpell.h"
#include "Actor/BaseProjectile.h"
#include "Interaction/CombatInterface.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"


void UBaseProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UBaseProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{


	// we want to spawn the projectile on the server and let it handle its movement. The client will see a replicated version of the projectile
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// we want a socket location from an actor that implements the combat interface, this way we can use this in different actors and not just the wolf (abstractions > specifics)
	const FVector SocketLocation =  ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	// Rotation.Pitch = 0.f; // ground parallel
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}


	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());


	// we want to make sure that the properties of the actor projectile are set before spawning it (need of gameplay effect, etc), thats why we use spawndeferred
	ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);  // always spawn regardless of collisions/overrides

	// Give the Projectile a Gameplay Effect Spec for causing Damage
	Projectile->SetOwner(GetAvatarActorFromActorInfo());
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();

	// Smart pointers are reference counted, as soon as an object has no references to it (pointers that point to it), it is automatically deleted by the Garbage Collector
	// Weak object pointers have the property of not being reference counted (reference counting to delete an object or extend its lifetime for that matter is not affected by a weakobject pointer) 
	// contexts arent necessarily tracked by GC in all cases so they need to be weak object pointers in the GameplayEffectTypes.h class (we avoid the GC deleting contexts when we need them)
	// our EffectContextHandle is a local variable, it is not tracked by garbage collection
	// this is optional to have more information on the context handle 
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		// we let the gameplay ability determine the damage of the gameplay effect based on the ability level hense Set By Caller (key (damage tag) - value pair) (we associate a key to a given value)
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());  // int 32 but requires a float but an implicit conversion is performed
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);
	
}
