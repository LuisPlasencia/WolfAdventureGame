// Made by Luis Plasencia Pulido


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"


// no F prefix, USTRUCT or generatedbody macro because we are not going to expose to blueprint or reflection system at all, it's just a raw internal struct for the cpp
struct BaseDamageStatics
{
	// macro for declaring attribute capture definitions (we dont have to pass in a variable, just text/tokens)
	// it creates an FProperty and an FGameplayEffectAttributeCaptureDefinition for us with the name of the token that we pass in
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	
	BaseDamageStatics()
	{
		// we want the armor of the target, not the source - in this case we dont want to snapshot hense the false
		// with this macro we define the FProperty (ArmorProperty) and the FGameplayEffectAttrivbuteCaptureDefinition (ArmorDef) that we declared with the previous macro
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// this will have the value of the captures attribute (out parameter)
	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(0.f, Armor);
	++Armor;

	// this is how we modify the attributes
	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
