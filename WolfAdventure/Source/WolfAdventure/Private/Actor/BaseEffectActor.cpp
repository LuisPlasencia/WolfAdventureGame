// Made by Luis Plasencia Pulido


#include "Actor/BaseEffectActor.h"
#include "Components/SphereComponent.h"
#include <AbilitySystemInterface.h>
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"



// Sets default values
ABaseEffectActor::ABaseEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());

}

void ABaseEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UBaseAttributeSet::StaticClass()));
		
		UBaseAttributeSet* MutableAuraAttributeSet = const_cast<UBaseAttributeSet*>(BaseAttributeSet);  // bypassess the const (bad practice but good for testing)
		MutableAuraAttributeSet->SetHealth(BaseAttributeSet->GetHealth() + 25.f);
		MutableAuraAttributeSet->SetMana(BaseAttributeSet->GetMana() - 25.f);
		Destroy();
	}
}

void ABaseEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseEffectActor::OnOverlap);  //we bind the callback (with the required input parameters or signature) to the dynamic multicast delegate OnComponentBeginOverlap
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABaseEffectActor::EndOverlap);
}


