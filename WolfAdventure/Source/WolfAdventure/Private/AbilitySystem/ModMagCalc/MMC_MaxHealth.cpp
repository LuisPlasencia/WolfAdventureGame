// Made by Luis Plasencia Pulido


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include <Interaction/CombatInterface.h>

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// we can do this thanks to the attribute accessors (macro), it is a static attribute so we dont need a pointer
	VigorDef.AttributeToCapture = UBaseAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	
	// whether we capture the value when the gameplay effect is created (original) or applied (true value at time of aplication) 
	VigorDef.bSnapshot = false;
	
	// we specify the attributes that we want to capture
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec,  EvaluationParameters, Vigor);
	// we clamp the captured value (no negative values)
	Vigor = FMath::Max<float>(Vigor, 0.f);

	int32 PlayerLevel = 1;
	// for implements we use the U version instead of the I version of the interface
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		// when we call the static execute version of the function we use the I version of the interface
		// since the execute version is static, it needs to know which object we are calling it on
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	// we return the custom calculation that we want for our modifier, in this case it depends on the player level as well as the vigor points
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
