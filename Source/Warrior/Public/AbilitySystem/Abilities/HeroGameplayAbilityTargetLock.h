// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbilityTargetLock.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroGameplayAbilityTargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	void CancelTargetLockAbility();
	void CleanUp();
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTracechannel;
	
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersisitentDebugShape = false;
	
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;
	
	UPROPERTY()
	AActor* CurrentLockedActor;
};
