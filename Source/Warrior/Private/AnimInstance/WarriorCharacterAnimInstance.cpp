// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/WarriorCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/WarriorBaseCharacter.h"
#include "KismetAnimationLibrary.h"

void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacterPtr = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacterPtr)
	{
		OwningMovementPtr = OwningCharacterPtr->GetCharacterMovement();
	}
}

void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (nullptr != OwningMovementPtr && nullptr != OwningCharacterPtr)
	{
		GroundSpeed = OwningCharacterPtr->GetVelocity().Size2D();
		bHasAcceleration = OwningMovementPtr->GetCurrentAcceleration().SizeSquared2D() > 0.f;
		LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacterPtr->GetVelocity(), OwningCharacterPtr->GetActorRotation());
	}
}
