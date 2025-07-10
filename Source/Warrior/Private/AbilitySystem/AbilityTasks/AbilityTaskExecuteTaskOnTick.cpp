// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTaskExecuteTaskOnTick.h"
UAbilityTaskExecuteTaskOnTick::UAbilityTaskExecuteTaskOnTick()
{
	bTickingTask = true;
}

UAbilityTaskExecuteTaskOnTick* UAbilityTaskExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwngingAbility)
{
	UAbilityTaskExecuteTaskOnTick* Node = NewAbilityTask<UAbilityTaskExecuteTaskOnTick>(OwngingAbility);
	return Node;
}


void UAbilityTaskExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}


