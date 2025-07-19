// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCountDownAction.h"

void FWarriorCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}

void FWarriorCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Cancelled;
		Response.FinishAndTriggerIf(true, ExecutetionFunction, OutputLink, CallbackTarget);
		return;
	}
	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutetionFunction, OutputLink, CallbackTarget);
		return;
	}
	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;
		CountDownOutput = EWarriorCountDownActionOutput::Updated;
		Response.TriggerLink(ExecutetionFunction, OutputLink, CallbackTarget);
		ElapsedInterval = 0.f;
	}
}
