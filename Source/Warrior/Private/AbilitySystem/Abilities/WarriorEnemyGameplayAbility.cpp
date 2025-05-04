// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"
#include "Characters/WarriorEnemyCharacter.h"

AWarriorEnemyCharacter* UWarriorEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CacheWarriorEnemyCharacterPtr.IsValid())
	{
		CacheWarriorEnemyCharacterPtr = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CacheWarriorEnemyCharacterPtr.IsValid() ? CacheWarriorEnemyCharacterPtr.Get() : nullptr;
}

UEnemyCombatComponent* UWarriorEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
