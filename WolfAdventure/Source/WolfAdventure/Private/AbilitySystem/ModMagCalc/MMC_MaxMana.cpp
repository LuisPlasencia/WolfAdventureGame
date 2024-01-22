// Made by Luis Plasencia Pulido


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include <Interaction/CombatInterface.h>

UMMC_MaxMana::UMMC_MaxMana()
{
	// we can do this thanks to the attribute accessors (macro), it is a static attribute so we dont need a pointer
	IntDef.AttributeToCapture = UBaseAttributeSet::GetIntelligenceAttribute();
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// whether we capture the value when the gameplay effect is created (original) or applied (true value at time of aplication) 
	IntDef.bSnapshot = false;

	// we specify the attributes that we want to capture
	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluationParameters, Int);
	// we clamp the captured value (no negative values)
	Int = FMath::Max<float>(Int, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	// we return the custom calculation that we want for our modifier, in this case it depends on the player level as well as the vigor points
	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
