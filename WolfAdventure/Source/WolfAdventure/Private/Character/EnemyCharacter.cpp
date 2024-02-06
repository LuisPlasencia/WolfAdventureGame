// Made by Luis Plasencia Pulido


#include "Character/EnemyCharacter.h"
#include <WolfAdventure/WolfAdventure.h>
#include <AbilitySystem/BaseAbilitySystemComponent.h>
#include "Components/WidgetComponent.h"
#include "UI/Widget/BaseUserWidget.h"
#include <AbilitySystem/BaseAttributeSet.h>
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "BaseGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	InitAbilityActorInfo();
	// only the server can access the game mode 
	if (HasAuthority())
	{
		UBaseAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}


	// we set the widget controller of the progress bar used by the widget component
	if (UBaseUserWidget* BaseUserWidget = Cast<UBaseUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		BaseUserWidget->SetWidgetController(this);
	}

	// we bind the lambdas to the callbacks after initializing the Ability System Component
	if (const UBaseAttributeSet* BaseAS = Cast<UBaseAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AEnemyCharacter::HitReactTagChanged
		);

		// we broadcast those initial values so the progress bar updates itself
		OnHealthChanged.Broadcast(BaseAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BaseAS->GetMaxHealth());
	}
}
void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// new count = 0 when we end the ability
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;   // if true 0 if false basewalkspeed

}

void AEnemyCharacter::InitAbilityActorInfo()
{

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	// only the server can access the game mode 
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}

}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UBaseAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AEnemyCharacter::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}

int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);

	Super::Die();
}
