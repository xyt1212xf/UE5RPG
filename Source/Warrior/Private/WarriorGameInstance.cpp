// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorGameInstance.h"

TSoftObjectPtr<UWorld> UWarriorGameInstance::GetGameLevelByTag(FGameplayTag InGameplayTag) const
{
	for (const FWarriorGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (GameLevelSet.LevelTag.IsValid())
		{
			if (InGameplayTag == GameLevelSet.LevelTag)
			{
				return GameLevelSet.Level;
			}
		}
	}
	return TSoftObjectPtr<UWorld>();
}
