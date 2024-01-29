// Made by Luis Plasencia Pulido


#include "AbilitySystem/Abilities/BaseProjectileSpell.h"
#include "Actor/BaseProjectile.h"
#include "Interaction/CombatInterface.h"

void UBaseProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);



}

void UBaseProjectileSpell::SpawnProjectile()
{


	// we want to spawn the projectile on the server and let it handle its movement. The client will see a replicated version of the projectile
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;


	// we want a socket location from an actor that implements the combat interface, this way we can use this in different actors and not just the wolf (abstractions > specifics)
	FTransform SpawnTransform;
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		SpawnTransform.SetLocation(SocketLocation);
		// TODO: Set the Projectile Rotation

		// we want to make sure that the properties of the actor projectile are set before spawning it (need of gameplay effect, etc), thats why we use spawndeferred
		ABaseProjectile*
			Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);  // always spawn regardless of collisions/overrides

		// TODO: Give the Projectile a Gameplay Effect Spec for causing Damage


		Projectile->FinishSpawning(SpawnTransform);
	}
}
