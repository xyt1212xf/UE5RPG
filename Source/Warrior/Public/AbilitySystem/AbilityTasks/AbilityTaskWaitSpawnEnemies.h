// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTaskWaitSpawnEnemies.generated.h"


class AWarriorEnemyCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSapwnEnemiesDelegate, const TArray<AWarriorEnemyCharacter*>&, SpawnedEnemies);


/**
 * 
 */
UCLASS()
class WARRIOR_API UAbilityTaskWaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (DisplayName = "Wait Gameplay Event and Spawn Enemies", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", NumToSpawn = "1", RangdomSpawnRadius = "200"))
	static UAbilityTaskWaitSpawnEnemies* WaitSpawnEnemies(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
		int NumToSpawn,
		const FVector& SpawnOrigin,
		float RangdomSpawnRadius,
		const FRotator& SpawnRoation);

	UPROPERTY(BlueprintAssignable)
	FWaitSapwnEnemiesDelegate OnSpawnFinished;

	UPROPERTY(BlueprintAssignable)
	FWaitSapwnEnemiesDelegate DidNotSpawn;

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	void OnEnemyClassLoaded();

private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AWarriorEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRangdomSpawnRadius;
	FRotator CachedSpawnRoation;
	FDelegateHandle DelegateHandle;
};
