// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickUps/WarriorStoneBase.h"
#include "Characters/WarriorHeroCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"

void AWarriorStoneBase::Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevle)
{
	check(StoneGameplayEffectClass);
	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevle,
		AbilitySystemComponent->MakeEffectContext() );

	BP_OnStoneConsumed();
}

void AWarriorStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWarriorHeroCharacter* OverlappedHeroCharacter = Cast<AWarriorHeroCharacter>(OtherActor))
	{
		OverlappedHeroCharacter->GetWarriorAbilitySystemComponent()->TryActivateAbilityByTag(WarriorGameplayTags::Player_Ability_PickUp_Stones);
	}
}
