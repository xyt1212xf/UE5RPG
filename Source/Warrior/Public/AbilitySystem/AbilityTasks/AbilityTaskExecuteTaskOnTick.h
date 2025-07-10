// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTaskExecuteTaskOnTick.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class WARRIOR_API UAbilityTaskExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTaskExecuteTaskOnTick();

	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (HidePin = "OwningAbility", DefaultToSelf ="OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTaskExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	virtual void TickTask(float DeltaTime)override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};



