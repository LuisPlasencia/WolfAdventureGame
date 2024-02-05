#pragma once  // so this header cant be included more than once anywhere
#include "GameplayEffectTypes.h"
#include "BaseAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FBaseGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

		// structs have a public body by default
public:

	bool IsCriticalHit() const { return bIsCriticalHit; }

	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	// A UClass is generated for each class in the engine that's derived from UObject for the reflection system,
	// structs also have a version of this called the ScriptStruct so when we create a struct that's capable of being exposed to the reflection system, a script struct is created for the reflection system
	/** Returns the serialization data, must always be overridden */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	// determines how the struct is serialized (serialization is necessary for saving structs or sending them across the network, it converts the function to binary)
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);


protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};