// Made by Luis Plasencia Pulido

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "BaseInputConfig.h"
#include "BaseInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class WOLFADVENTURE_API UBaseInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	// we dont forward declare baseinputconfig because we want it to be a template function
	// Template function capable of receiving functions or function pointers whatever the function signature is
	// A typename keyword tells the compiler that an identifier is a type (rather than a static member variable), For naming template parameters, typename and class are equivalent
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UBaseInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UBaseInputComponent::BindAbilityActions(const UBaseInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FBaseInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				// started will call the function (HeldFunc callback function) once when (Action.InputAction) is pressed. Action.InputTag will be passed along to our callback heldFunc function and we can add more arguments (opcional)
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				// completed will call the function (HeldFunc callback function) once when (Action.InputAction) is released. Action.InputTag will be passed along to our callback heldFunc function and we can add more arguments (opcional)
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				// triggered will call the function (HeldFunc callback function) every frame as long as the input (Action.InputAction) is held. Action.InputTag will be passed along to our callback heldFunc function and we can add more arguments (opcional)
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}


		}
	}
}
