// Made by Luis Plasencia Pulido


#include "Actor/BaseEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <Kismet/KismetMathLibrary.h>



// Sets default values
ABaseEffectActor::ABaseEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//SetRootComponent(Mesh);

	//Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	//Sphere->SetupAttachment(GetRootComponent());

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void ABaseEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	const float SinePeriod = 2 * PI / SinePeriodConstant;
	if (RunningTime > SinePeriod)
	{
		RunningTime = 0.f;
	}
	ItemMovement(DeltaTime);
}

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();

	//Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseEffectActor::OnOverlap);  //we bind the callback (with the required input parameters or signature) to the dynamic multicast delegate OnComponentBeginOverlap
	//Sphere->OnComponentEndOverlap.AddDynamic(this, &ABaseEffectActor::EndOverlap);
}

void ABaseEffectActor::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedRotation = GetActorRotation();
}

void ABaseEffectActor::StartRotation()
{
	bRotates = true;
	CalculatedRotation = GetActorRotation();
	InitialLocation = GetActorLocation();
}

void ABaseEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	//if (ASCInterface)
	//{
	//	ASCInterface->GetAbilitySystemComponent();
	//}

	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;


	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle =  TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;  // Def is the gameplay effect from the gameplayEffectSpec
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	if (!bIsInfinite)
	{
		Destroy();
	}
}

void ABaseEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

}

void ABaseEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

void ABaseEffectActor::ItemMovement(float DeltaTime)
{
	if (bRotates)
	{
		const FRotator DeltaRotation(0.f, DeltaTime * RotationRate, 0.f);
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation); // the same effect as asdding them together
	}
	if (bSinusoidalMovement)
	{
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
	}
}


//void ABaseEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
//	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
//	{
//		const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UBaseAttributeSet::StaticClass()));
//		
//		UBaseAttributeSet* MutableAuraAttributeSet = const_cast<UBaseAttributeSet*>(BaseAttributeSet);  // bypassess the const (bad practice but good for testing)
//		MutableAuraAttributeSet->SetHealth(BaseAttributeSet->GetHealth() + 25.f);
//		MutableAuraAttributeSet->SetMana(BaseAttributeSet->GetMana() - 25.f);
//		Destroy();
//	}
//}
//
//void ABaseEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//
//}





