// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/WarriorBaseAnimInstance.h"
#include "WarriorCharacterAnimInstance.generated.h"
class AWarriorBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()
public:
	//需要实现这2个函数，这2个函数用来获取到MovementComponent和Character
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AWarriorBaseCharacter* OwningCharacterPtr;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementPtr;

	//速度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocalmotionData")
	float GroundSpeed;

	//是否有加速度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocalmotionData")
	bool bHasAcceleration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocalmotionData")
	float LocomotionDirection;
};
