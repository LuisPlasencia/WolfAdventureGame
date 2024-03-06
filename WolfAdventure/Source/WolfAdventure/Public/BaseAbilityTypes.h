#pragma once  // so this header cant be included more than once anywhere
#include "GameplayEffectTypes.h"
#include "BaseAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	// because we are passing this struct over to baseAbilitySystemLibrary we need a world context
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;
};

// the effect context lives with the gameplay effect and carries along information
USTRUCT(BlueprintType)
struct FBaseGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

		// structs have a public body by default
public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; };
	float GetDebuffDamage() const { return DebuffDamage; };
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }

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

	// NECESSARY FOR REPLICATION
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

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	// garbage collection is not handled for shared pointers by UPROPERTY. Smart pointers like shared pointer handles the automatic memory management (this is why we skip uproperty here)
	// on the other hand, weak object pointers are not reference counted unlike smart pointers 
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
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