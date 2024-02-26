// Made by Luis Plasencia Pulido


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "WolfAdventure/BaseLogChannels.h"
#include <AbilitySystem/Abilities/BaseGameplayAbility.h>
#include <Interaction/PlayerInterface.h>
#include <AbilitySystemBlueprintLibrary.h>

void UBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	// we make this callback function a client RPC since, OnGameplayEffectAppliedDelegateToSelf is only called on server and not on clients
	// client RPC (Remote procedure call): it will be called on the server but also executed on owning clients
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilitySystemComponent::ClientEffectApplied);  // not add dynamic since it is not a dynamic delegate
	


//	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
}

void UBaseAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
		{
			// dynamic ability tags are designed to be added and removed at RUNTIME
			// specific to the player controlled character, enemies dont need to care about their input tag
			AbilitySpec.DynamicAbilityTags.AddTag(BaseAbility->StartupInputTag);
			// give ability can accept const abilitySpec but giveabilityandactivateonce only accepts non-const
			//GiveAbilityAndActivateOnce(AbilitySpec);
			GiveAbility(AbilitySpec);
		}
	}
	// remember that this is only called on the server. For the client side, the OnRep_ActivateAbilities() virtual RPC (remote procedure call - virtual rep notify) function will get called and we will broadcast there for the clients
	// the abilitysystemcomponent Activatable abilities container replicates (replicated variable) using OnRep_ActivateAbilities once we call giveAbility aka the container changes
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UBaseAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// we tell the ability that its input is being pressed
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				// we try to activate the ability, since it may be blocked by other abilities
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	// the ability should determine if it should be cancelled or ended when the ability is no longer pressed

	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// we tell the ability that its input is being released
			AbilitySpecInputReleased(AbilitySpec);
		}
	}

}

void UBaseAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	// (+Good Practice) this is how we lock the list (activatable abilities container) until we have finished our for loop just in case anything changes while we are looping (abilities can change status and become blocked or non activatable by some gameplay tag etc)
	// it will keep track of any abilities that are attempted to be removed or added and wait until this scope has finished before mutating the list (this is why we manage this in the ability system componenent rather than outside of it, in the overlaywidgetcomponent f.e.)
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogBase, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);  // we log the name of the function
		}

	}
}

FGameplayTag UBaseAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// for a given ability spec, we are going to look at that abilityspec's ability tags and assuming it only has one ability tag that has "abilities" in it, we are going to return that tag
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	
	return FGameplayTag();
}

FGameplayTag UBaseAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UBaseAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	// we dont want to make the ASC dependant on the playerstate so we use an interface 
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UBaseAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	// we send an event. A gameplay ability will listen for an event with this tag and apply a gameplay affect using information in that event
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UBaseAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}

}

void UBaseAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
//	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);

	//for (const FGameplayTag& Tag : TagContainer)  // a reference so we dont copy the tag (+efficiency)
	//{
	//	// TODO: Broadcast the tag to the Widget Controller
	//	const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	//	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
	//}
}
