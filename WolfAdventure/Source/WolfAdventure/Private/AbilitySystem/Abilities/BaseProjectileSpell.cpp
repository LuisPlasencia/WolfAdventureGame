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

void UBaseProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{


	// we want to spawn the projectile on the server and let it handle its movement. The client will see a replicated version of the projectile
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// we want a socket location from an actor that implements the combat interface, this way we can use this in different actors and not just the wolf (abstractions > specifics)
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		// Rotation.Pitch = 0.f; // ground parallel

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());


		// we want to make sure that the properties of the actor projectile are set before spawning it (need of gameplay effect, etc), thats why we use spawndeferred
		ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);  // always spawn regardless of collisions/overrides

		// Give the Projectile a Gameplay Effect Spec for causing Damage
		Projectile->SetOwner(GetAvatarActorFromActorInfo());
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		
		const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
		// we let the gameplay ability determine the damage of the gameplay effect based on the ability level hense Set By Caller (key (damage tag) - value pair) (we associate a key to a given value)
		const float ScaledDamage = Damage.GetValueAtLevel(10);  // int 32 but requires a float but an implicit conversion is performed
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
