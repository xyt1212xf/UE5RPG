// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTaskWaitSpawnEnemies.h"

UAbilityTaskWaitSpawnEnemies* UAbilityTaskWaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn, int NumToSpawn, const FVector& SpawnOrigin, float RangdomSpawnRadius, const FRotator& SpawnRoation)
{
	UAbilityTaskWaitSpawnEnemies* NodePtr = NewAbilityTask<UAbilityTaskWaitSpawnEnemies>(OwningAbility);
	NodePtr->CachedEventTag = EventTag;
	NodePtr->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	NodePtr->CachedNumToSpawn = NumToSpawn;
	NodePtr->CachedSpawnOrigin = SpawnOrigin;
	NodePtr->CachedRangdomSpawnRadius = RangdomSpawnRadius;
	NodePtr->CachedSpawnRoation = SpawnRoation;
	return NodePtr;
}
