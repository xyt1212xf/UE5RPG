// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/WarriorBaseAnimInstance.h"
#include "WarriorHeroLinkedLayer.generated.h"


class UWarriorHeroAnimInstance;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroLinkedLayer : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UWarriorHeroAnimInstance* GetLinkedAnimInstance() const;

};
