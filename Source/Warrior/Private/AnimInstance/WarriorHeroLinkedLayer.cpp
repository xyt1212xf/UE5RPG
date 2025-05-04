// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/WarriorHeroLinkedLayer.h"
#include "AnimInstance/WarriorHeroAnimInstance.h"

UWarriorHeroAnimInstance* UWarriorHeroLinkedLayer::GetLinkedAnimInstance() const
{
	return Cast<UWarriorHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}

