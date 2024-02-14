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
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		// the projectile can be destroyed before it can construct a loopingsoundcomponent 
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
		bHit = true;
	}
	Super::Destroyed();
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;
	// on clients, the damageEffectSpecHandle data is not valid, since in spawnProjectile (BaseProjectileSpell class), we make sure to only set the spec handle on the server and it is not a replicated variable
	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}
	if (!UBaseAbilitySystemLibrary::IsNotFriend(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return;
	}
	if (!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
		bHit = true;
	}

	// there is a chance that Destroy could replicate down to the client before the client has the function sphereoverlap is called (destroyed before sound and niagara) thats why we use the boolean
	if (HasAuthority())
	{
		// the effect is going to change an attribute and the attribute is replicated to clients 
		if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// we set this damage effect spec handle on projectile spawn in baseprojectilespell ability. It has a value associated with a level in the dataAsset as an scalablefloat
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}

		Destroy();
	}
	else
	{
		bHit = true;
	}
}


