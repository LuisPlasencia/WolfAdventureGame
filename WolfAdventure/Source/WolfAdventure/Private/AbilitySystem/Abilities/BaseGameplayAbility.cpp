// Made by Luis Plasencia Pulido


#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystem/BaseAttributeSet.h"

FString UBaseGameplayAbility::GetDescription(int32 Level)
{
    // L = string literal, concatenation - wide character
    // LorempIpsum = text that we dont matter what it says
    return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UBaseGameplayAbility::GetNextLevelDescription(int32 Level)
{
    // \n = new line character
    return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage </>"), Level);
}

FString UBaseGameplayAbility::GetLockedDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

float UBaseGameplayAbility::GetManaCost(float InLevel) const 
{
    float ManaCost = 0.f;
    if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
    {
        for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
        {
            if (Mod.Attribute == UBaseAttributeSet::GetManaAttribute())
            {
                // only works with scalable floats (hardcoded value or value scaled by a curve table's curve
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
                break;
            }
        }
    }
    return ManaCost;
}

float UBaseGameplayAbility::GetCooldown(float InLevel) const
{
    float Cooldown = 0.f;
    if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
    {
        // we dont have modifiers in our coolodown effects, its just a duration based gameplay effect with a magnitude
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
    }
    return Cooldown;
}
