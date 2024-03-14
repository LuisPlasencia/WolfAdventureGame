// Made by Luis Plasencia Pulido


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "BaseGameplayTags.h"
#include <AbilitySystem/Data/CharacterClassInfo.h>
#include <AbilitySystem/BaseAbilitySystemLibrary.h>
#include <Interaction/CombatInterface.h>
#include <BaseAbilityTypes.h>
#include <Kismet/GameplayStatics.h>

// damage execution calculations can be predicted
// ExecCalcs can mmodify multiple attributes, but MMCs cannot
// no F prefix, USTRUCT or generatedbody macro because we are not going to expose to blueprint or reflection system at all, it's just a raw internal struct for the cpp
struct BaseDamageStatics
{
	// macro for declaring attribute capture definitions (we dont have to pass in a variable, just text/tokens)
	// it creates an FProperty and an FGameplayEffectAttributeCaptureDefinition for us with the name of the token that we pass in
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);


	BaseDamageStatics()
	{
		// we want the armor of the target, not the source - in this case we dont want to snapshot hense the false
		// with this macro we define the FProperty (ArmorProperty) and the FGameplayEffectAttrivbuteCaptureDefinition (ArmorDef) that we declared with the previous macro
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration, Source, false);  // armor penetration needs to be captured from the source character since it is the one doing the damage (attacker)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalResistance, Target, false);

	}
};


static const BaseDamageStatics& DamageStatics()
{
	// static object since we want to access the same one everytime (no dynamic memory allocations as with pointers but a static one) 
	// (it doesnt go away once the function ends, it will still be the same even though it is not dynamically allocated on the heap with the 'new' keyword)
	// every time we call this function we will return the same DStatics object
	static BaseDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// we tell the execution calculation class that this capture def should be used to capture that specific attribute (in MMC_MaxHealth for example, we do the process of creating the AttributeDef manually without macros, but it's the same thing)
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
							const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f) // .5 padding for floating point [im]precision
		{
			// Determine if there was a successful debuff
			// HEY!  We set the debuff chance, etc.. with AssignTagSetByCallerMagnitude and we get them WITH getsetbycallermagnitude but resistance f.e. is an attribute based magnitude so it is captured with our damage statics capture definitions!!
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);

			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{

				// local variable - not tracked by garbage collection .... its weakobjectptr variables are not tracked by GC aither unlike smart pointers
				// hey!! this is a context handle, it is not the gameplayeffectcontext itself!! to access it we need to call Get() which we do in the baseabilitysystemlibrary functions! .Get() returns a pointer to the ACTUAL context. The handle is merely a wrapper
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UBaseAbilitySystemLibrary::SetIsSuccessfullDebuff(ContextHandle, true);
				UBaseAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);
				
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);

				UBaseAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UBaseAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UBaseAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// to know which capture def is associated with which resistance tag
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FBaseGameplayTags& Tags = FBaseGameplayTags::Get();

	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);


	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	// local variable - not tracked by garbage collection .... its weakobjectptr variables are not tracked by GC aither unlike smart pointers
	// hey!! this is a context handle, it is not the gampelayeffectcontext itself!! to access it we need to call Get() which we do in the baseabilitysystemlibrary functions!
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);


	// Get Damage Set by Caller Magnitude    (the ability in question does the assignstagsetbycallermagnitude depending on the ability (fire, electricity...)) and here we capture those values 
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FBaseGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);  // returns 0 if not set for a given damage type
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;  // we reduce the damage type value by the resistance to that damage type in percentage
		

		if (UBaseAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 1. override TakeDamage in CharacterBase *
			// 2. create delegate OnDamageDelegate, broadcast damage received in TakeDamage *
			// 3. Bind lambda to OnDamageDelegate on the Victim here. * 
			// 4. Call UGameplayStatics::ApplyRadialDamageWithFalloff to cause damage (this will result in TakeDamage being called
			//		on the Victim, which will then broadcast OnDamageDelegate) *
			// 5. In Lambda, set DamageTypeValue to the damage received from the broadcast *

			// we avoid this class depending on characterbase by getting the delegate through an interface.
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				// the & in the capture brackets captures everything by reference.
				CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount) 
				{
					// we are not overriding the resistance calculations because we already took them into acount when we called the radial damage function.
					DamageTypeValue = DamageAmount;

				});
			}

			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar, 
				DamageTypeValue,
				0.f,
				UBaseAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UBaseAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UBaseAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}
		
		Damage += DamageTypeValue;

	}

	// Capture BlockChance on Target and determine if there was a successful Block

	// this will have the value of the captured attribute (out parameter)
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	// in this case we are going to apply the block before any crit damage, armor penetration or damage boost (design choice)
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;


	// extracting the methods to the library saves us the trouple of accessing the base gameplay effect context each time on different classes
	UBaseAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);


	// If Block, halve the damage
	Damage = bBlocked ? Damage / 2.f : Damage;

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);


	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = UBaseAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	// ArmorPenetration ignores a percentage of the Target's Armor
	// we scale armor penetration so that 1 armor penetration ignores x% of armor and x armor pen ignores 100% armor, we scale the damage calculation coefficients depending on the level of the enemy with a curve table (ex: x0.25 from 1 to 30 - x0.15 from 30 to 60)
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	// Armor ignores a percentage of incoming Damage
	// we scale armor to damage conversion so that each point of the effective armor reduces damage by x% (these values can be changed in the curve table as well)
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UBaseAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// Double damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	// this is how we modify the attributes
	// in this case we are making our evaluatedData modify the incoming damage meta attribute on the target, it takes an FGameplayAttribute and DamageStatics can 
	// probide that for us if we are capturing the attribute but in this case we are not capturing incoming damage on the target but we know the getters for the attributes are static functions
	// we are adding damage to the incoming damage meta attribute of the target
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}


