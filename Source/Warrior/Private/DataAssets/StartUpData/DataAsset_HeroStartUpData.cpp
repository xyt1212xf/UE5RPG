// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel /*= 1*/)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	for (const auto& AbilitySet : HeroStartUpAbilitySets)
	{
		if (AbilitySet.IsValid())
		{
			FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
