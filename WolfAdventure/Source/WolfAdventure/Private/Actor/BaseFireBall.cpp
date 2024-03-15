// Made by Luis Plasencia Pulido


#include "Actor/BaseFireBall.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "GameplayCueManager.h"
#include "Components/AudioComponent.h"
#include <AbilitySystem/BaseAbilitySystemLibrary.h>
#include <BaseGameplayTags.h>

void ABaseFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void ABaseFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	 
	// there is a chance that Destroy could replicate down to the client before the client has the function sphereoverlap is called (destroyed before sound and niagara) thats why we use the boolean
	if (HasAuthority())
	{
		// the effect is going to change an attribute and the attribute is replicated to clients 
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			//// we set this damage effect spec handle on projectile spawn in baseprojectilespell ability. It has a value associated with a level in the dataAsset as an scalablefloat
			//TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void ABaseFireBall::OnHit()
{
	if (GetOwner())
	{
		// we execute a local gameplay cue. This is not an RPC in contrast to the normal ones so we wont oversaturate the network with a gameplay cue per fireball (too expensive bandwidth wise), 
		// that would be too much information sent across the server since we set the gameplay cue limit to 10 at the same time in project settings. (Config/DefaultEngine.ini)
		// we specified our project's gameplay cues folder as well to content/blueprints/abilitysystem/gameplaycuenotifies so the engine can find them faster. (Config/DefaultGame.ini)
		// if something is already replicated / predicted, local gameplay cues are really useful. 
		// Normal gameplay cues (multicast RPC) are great for spawning particles effects and sounds on clients so that other clients can see them, since these are not replicated by default.
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FBaseGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent(); // in case the projectile doesnt hit anything, the looping component may stick around so we want to destroy it
	}

	bHit = true;
}
