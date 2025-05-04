// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InputConfigPtr, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserObject* ContextObjectPtr, CallbackFunc Func)
	{
		checkf(InputConfigPtr, TEXT("input config data asset is null, can not processed with binding"));
		if (UInputAction* FoundAction = InputConfigPtr->FindNativeInputActionByTag(InputTag))
		{
			BindAction(FoundAction, TriggerEvent, ContextObjectPtr, Func);
		}
	}	
	
	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InputConfigPtr, UserObject* ContextObjectPtr, CallbackFunc InputPressedFunc, CallbackFunc InputReleaseFunc)
	{
		checkf(InputConfigPtr, TEXT("input config data asset is null, can not processed with binding"));
		for (const auto&  AbilityInputActionConfig : InputConfigPtr->AbilityInputActions)
		{
			if (AbilityInputActionConfig.IsValid())
			{
				BindAction(AbilityInputActionConfig.InputActionPtr, ETriggerEvent::Started, ContextObjectPtr, InputPressedFunc, AbilityInputActionConfig.InputTag);
				BindAction(AbilityInputActionConfig.InputActionPtr, ETriggerEvent::Completed, ContextObjectPtr, InputReleaseFunc, AbilityInputActionConfig.InputTag);
			}
		}
		
	}
};
