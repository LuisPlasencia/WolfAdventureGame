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
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FBaseGameplayEffectContext* Duplicate() const
	{
		FBaseGameplayEffectContext* NewContext = new FBaseGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// determines how the struct is serialized (serialization is necessary for saving structs or sending them across the network, it converts the function to binary)
	// UPackageMap is a tool to help map objects to indices (when serializing all variables from the struct are converted to strings of ceros and ones (array of bits), and it helps to know when an object starts and the other begins with UPackageMap) 
	// FArchive is a byte class that uses a byte order neutral way (big endian) and is capable of storing serialized data (<< operator overload in archive class (on the left hand sign what it returns and on the right hand side the parameter in the function))
	// << operator works both ways not just to the left, it converts a defined variable type from or into an archive (it can load left to right or save right to left) (left = bits , right = some type variable)
	// depending whether the archive is storing or saving,  (RepBits |= 1 << 0 ) -> repbits Bitwise OR (|) equals (repbits = repbits | 1 << 0 ) 1 left shift (<<) 0 -> Repbits is unsigned 8 bit integer (0000 0000)
	// ( | is a bitwise OR ) ( << is a Shift Left operator, 1<<0 would mean shift all bits to the left by 0 (stays the same), 1<<1 shifts all bits to the left by 1, 1<<2 shifts to the left by 2 (we lose the 2 bits on the far left with the shift because they get pushed))
	// an standard integer is 32 bits, RepBits |= 1 <<0 or RepBits = RepBits | 1<<0  flips the 0th bit, RepBits will tell us whether or not we should serialize x variables depending on the bit position, x position represents a variable, 1 means replicate 0 means dont replicate because it is not valid or is empty
	//  & is a bitwise AND (masks), we use it to know whether we set a 0 or a 1 in x position  
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);


protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};


// we need to set true some qualities that are important for serialization and for the reflection system (custom aspects of a script struct - used for serialization and for the reflection system)
// defines what can be done with FBaseGameplayEffectContext - StructOps = Determines what the Script Struct is capable of,  Script Struct = Version of a structs that represents it in the reflection system (much like UClass for classes)
template<>
struct TStructOpsTypeTraits<FBaseGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FBaseGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};