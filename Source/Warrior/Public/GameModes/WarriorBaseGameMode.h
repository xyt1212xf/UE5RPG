// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarriorEnumType.h"

#include "WarriorBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWarriorBaseGameMode();
	EWarriorGameDifficult GetCurrentWarriroGameDifficult() const
	{
		return CurrentGameDifficulty;
	}
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings");
	EWarriorGameDifficult CurrentGameDifficulty;
};
