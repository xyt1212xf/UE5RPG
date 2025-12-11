// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WarriorBaseGameMode.h"

AWarriorBaseGameMode::AWarriorBaseGameMode()
{
	TArray<int >a ;
	a.Push(222);
	a.SetNum(10);
	TArray<int >b;
	b.Push(222);
	b.SetNum(10);

	auto xx = TNumericLimits<int32>::Max();
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}
