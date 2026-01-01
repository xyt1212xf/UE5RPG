// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "WarriorGameplayTags.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (InInputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
			{
				if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
				{
					CancelAbilityHandle(AbilitySpec.Handle);
				}
				else
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}


void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
	{
		return;
	}
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, 
	const TArray<FWarriorHeroSpecialAbilitySet>& InSpecialWeaponAbilities,
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles,
	int32 ApplyLevel)
{
	//if (InDefaultWeaponAbilities.IsEmpty())
	//{
	//	return;
	//}
	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}

	for (const FWarriorHeroAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UWarriorAbilitySystemComponent::RemoveGrantHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesRemove)
{
	if (InSpecHandlesRemove.IsEmpty())
	{
		return;
	}
	for (FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesRemove)
	{
		ClearAbility(SpecHandle);
	}
	InSpecHandlesRemove.Empty();
}

bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate, bool bAllowRemoteActivation /*= true*/)
{
	check(AbilityTagToActivate.IsValid());
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(),
		FoundAbilitySpecs);
	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandodmAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivatePtr = FoundAbilitySpecs[RandodmAbilityIndex];
		check(SpecToActivatePtr);
		if (!SpecToActivatePtr->IsActive())
		{
			return TryActivateAbility(SpecToActivatePtr->Handle);
		}
	}
	return false;
}


