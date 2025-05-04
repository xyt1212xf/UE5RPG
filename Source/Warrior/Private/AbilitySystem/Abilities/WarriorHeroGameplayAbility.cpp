// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"

AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CacheWarriorHeroCharacterPtr.IsValid())
	{
		CacheWarriorHeroCharacterPtr = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CacheWarriorHeroCharacterPtr.IsValid() ? CacheWarriorHeroCharacterPtr.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CacheWarriorHeroControllerPtr.IsValid())
	{
		CacheWarriorHeroControllerPtr = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
	}
	return CacheWarriorHeroControllerPtr.IsValid() ? CacheWarriorHeroControllerPtr.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}
