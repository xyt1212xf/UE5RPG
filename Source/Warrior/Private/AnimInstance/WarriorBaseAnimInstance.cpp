// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/WarriorBaseAnimInstance.h"
#include "WarriorFunctionLibrary.h"

bool UWarriorBaseAnimInstance::DoesOwnerHaveGameTag(FGameplayTag TagToCheck) const
{
	bool bResult = false;
	if (APawn* OwnerPawn = TryGetPawnOwner())
	{
		bResult = UWarriorFunctionLibrary::NativeDoesActorHaveTag(OwnerPawn, TagToCheck);
	}
	return bResult;
}
