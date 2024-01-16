// Made by Luis Plasencia Pulido


#include "Actor/BaseEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"



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

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();

	//Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseEffectActor::OnOverlap);  //we bind the callback (with the required input parameters or signature) to the dynamic multicast delegate OnComponentBeginOverlap
	//Sphere->OnComponentEndOverlap.AddDynamic(this, &ABaseEffectActor::EndOverlap);
}

void ABaseEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	//if (ASCInterface)
	//{
	//	ASCInterface->GetAbilitySystemComponent();
	//}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle =  TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

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





