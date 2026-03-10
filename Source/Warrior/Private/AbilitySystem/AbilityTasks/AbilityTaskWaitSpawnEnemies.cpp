// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTaskWaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Characters/WarriorEnemyCharacter.h"

UAbilityTaskWaitSpawnEnemies* UAbilityTaskWaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn, int NumToSpawn, const FVector& SpawnOrigin, float RangdomSpawnRadius)
{
	UAbilityTaskWaitSpawnEnemies* NodePtr = NewAbilityTask<UAbilityTaskWaitSpawnEnemies>(OwningAbility);
	NodePtr->CachedEventTag = EventTag;
	NodePtr->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	NodePtr->CachedNumToSpawn = NumToSpawn;
	NodePtr->CachedSpawnOrigin = SpawnOrigin;
	NodePtr->CachedRangdomSpawnRadius = RangdomSpawnRadius;
	return NodePtr;
}

void UAbilityTaskWaitSpawnEnemies::Activate()
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTaskWaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	Delegate.Remove(DelegateHandle);
	//这个一定要最后调用
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTaskWaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded));
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}
		EndTask();
	}
}

void UAbilityTaskWaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* WorldPtr = GetWorld();
	if (!LoadedClass||!WorldPtr)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}
		EndTask();
		return;
	}
	
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray<AWarriorEnemyCharacter*> SpawnedEnemies;
	for (int32 i = 0; i < CachedNumToSpawn; ++i)
	{
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(
			this,
			CachedSpawnOrigin,
			RandomLocation,
			CachedRangdomSpawnRadius);
		RandomLocation += FVector(0.f, 0.f, 150.f);
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();
		AWarriorEnemyCharacter* SpawnEnemy = WorldPtr->SpawnActor<AWarriorEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParam);
		if (SpawnEnemy)
		{
			SpawnedEnemies.Add(SpawnEnemy);
		}
	}
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}
	}
	EndTask();
}
