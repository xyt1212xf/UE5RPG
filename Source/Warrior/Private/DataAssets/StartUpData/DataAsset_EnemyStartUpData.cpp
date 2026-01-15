// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/StartupData/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UWarriorEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityClass);
				AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
				AbilitySpec.Level = ApplyLevel;

				InASCToGive->GiveAbility(AbilitySpec);
			}
		}
	}
}
