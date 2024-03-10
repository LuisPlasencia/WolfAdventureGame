// Made by Luis Plasencia Pulido



#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystem/BaseAbilitySystemComponent.h>
#include "Components/CapsuleComponent.h"
#include "BaseGameplayTags.h"
#include <WolfAdventure/WolfAdventure.h>
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// our character base is dependant on debuffniagaracomponent, which is a good reason to not let debuffniagaracomponent be dependant on characterbase (we dont want circular dependencies). Thats why we use an interface
	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FBaseGameplayTags::Get().Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FBaseGameplayTags::Get().Debuff_Stun;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, bIsStunned);
	DOREPLIFETIME(ACharacterBase, bIsBurned);
	DOREPLIFETIME(ACharacterBase, bIsBeingShocked);
}

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ACharacterBase::Die(const FVector& DeathImpulse)
{
	if (Weapon != nullptr)
	{
		// detach is a replicated action by default (no need to detach on clients)
		Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	}

	MulticastHandleDeath(DeathImpulse);
}

void ACharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	if (Weapon != nullptr)
	{
		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true); // bVelChange to true to not take the mass into acount (so we dont need big impulse numbers), weapons are lighter than the mesh thats why we scale
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true); // bVelChange to true to not take the mass into acount (so we dont need big impulse numbers)
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();

	// Because this function is an RPC, even though bDead is not set to isReplicable, because we are changing it inside an RPC, it will replicate to all clients as well (clients and server)
	bDead = true;
	BurnDebuffComponent->Deactivate();
	StunDebuffComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
}

void ACharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// new count = 0 when we end the ability
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;   // if true 0 if false basewalkspeed
}

void ACharacterBase::OnRep_Stunned()
{
	
}

void ACharacterBase::OnRep_Burned()
{
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ACharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	// Return correct socket based on MontageTag
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Bite))
	{
		return GetMesh()->GetSocketLocation(BiteSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

bool ACharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ACharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ACharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* ACharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage ACharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ACharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ACharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass ACharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& ACharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeathSignature& ACharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

USkeletalMeshComponent* ACharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void ACharacterBase::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock;
}

bool ACharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ACharacterBase::AddCharacterAbilities()
{
	UBaseAbilitySystemComponent* BaseASC = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);

	// Should only add abilities on the server and be replicated down to clients
	if (!HasAuthority()) return;

	BaseASC->AddCharacterAbilities(StartupAbilities);
	BaseASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);

}

void ACharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);

		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);

		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}
