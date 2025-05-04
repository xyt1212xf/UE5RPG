// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/boxComponent.h"
#include "WarriorDebugHelper.h"

void UPawnCombatComponent::RegisterSpawndWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon /*= false*/)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named named %s has aleady been added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);


	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}

	const FString WeaponString = FString::Printf( TEXT("A weapon named: %s han been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	Debug::Print(WeaponString);
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		auto* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet);
		return *FoundWeapon;
	}
	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEanble, EToggleWeaponType InToggleWeaponType)
{
	if (EToggleWeaponType::CurrentWeapon == InToggleWeaponType)
	{
		AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
		check(WeaponToToggle);
		if (bShouldEanble)
		{
			WeaponToToggle->GetWeaponCollisionBoxPtr()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBoxPtr()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
		}

	}
	else if (EToggleWeaponType::LeftHand == InToggleWeaponType)
	{
	
	}
	else if (EToggleWeaponType::RightHand == InToggleWeaponType)
	{
	
	}
	else
	{
		checkf(false, TEXT("Invalid toggle weapon type"));
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
