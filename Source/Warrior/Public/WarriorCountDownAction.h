// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WarriorEnumType.h"
/**
 * 
 */
class  FWarriorCountDownAction : public FPendingLatentAction
{
public:
	FWarriorCountDownAction( float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime, EWarriorCountDownActionOutput& InCountDownOutput, const FLatentActionInfo& LatentInfo)
	: TotalCountDownTime(InTotalCountDownTime)
	, UpdateInterval(InUpdateInterval)
	, OutRemainingTime(InOutRemainingTime)
	, CountDownOutput(InCountDownOutput)
	, ExecutetionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
	{

	}

	void CancelAction();
	
	virtual void UpdateOperation(FLatentResponse& Response) override;
	
private:
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EWarriorCountDownActionOutput& CountDownOutput;
	FName ExecutetionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bNeedToCancel = false;
	float ElapsedInterval = 0.f;
	float ElapsedTimeSinceStart = 0.f;
};
