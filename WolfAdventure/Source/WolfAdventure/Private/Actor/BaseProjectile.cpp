// Made by Luis Plasencia Pulido


#include "Actor/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include <WolfAdventure/WolfAdventure.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"
#include <AbilitySystem/BaseAbilitySystemLibrary.h>


ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// we want to spawn the projectile on the server and let it handle its movement. The client will see a replicated version of the projectile
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}


void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnSphereOverlap);

	// THE AUDIO COMPONENT doesnt live on the actor, it flies around in the world following the actor in this case
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void ABaseProjectile::Destroyed()
{
	// there is a chance that Destroy could replicate down to the client before the client has the function sphereoverlap called (destroyed before sound and niagara) thats why we use the boolean
	if (!bHit && !HasAuthority())
	{
		OnHit();
		bHit = true;
	}
	Super::Destroyed();
}

void ABaseProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	// the projectile can be destroyed before it can construct a loopingsoundcomponent 
	if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	bHit = true;
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (OtherActor == GetOwner()) return;
	//// on clients, the damageEffectSpecHandle data is not valid, since in spawnProjectile (BaseProjectileSpell class), we make sure to only set the spec handle on the server and it is not a replicated variable
	//if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	//{
	//	return;
	//}

	if (SourceAvatarActor == OtherActor) return;
	if (!UBaseAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
	if (!bHit) OnHit();

	// there is a chance that Destroy could replicate down to the client before the client has the function sphereoverlap is called (destroyed before sound and niagara) thats why we use the boolean
	if (HasAuthority())
	{
		// the effect is going to change an attribute and the attribute is replicated to clients 
		if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			//// we set this damage effect spec handle on projectile spawn in baseprojectilespell ability. It has a value associated with a level in the dataAsset as an scalablefloat
			//TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				// we want to give the knockback some pitch override so the enemy flies around a little bit (angled up, inclination)
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();  // GetActorForwardVector().RotateAngleAxis(45.f, GetActorRightVector()) = alternative with not-complete pitch override (we are adding)
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		Destroy();
	}
	else bHit = true;
}


