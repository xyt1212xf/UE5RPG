// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "WarriorDebugHelper.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.0f);
	InitMaxHealth(1.0f);
	InitCurrentRage(1.0f);
	InitMaxRage(1.0f);
	InitAttackPower(1.0f);
	InitDefensePower(1.0f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("ERROR--->001"));

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("ERROR--->002"));

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);

		if (GetCurrentRage() == GetMaxRage())
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNode(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
		}
		else if(GetCurrentRage() == 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNode(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UWarriorFunctionLibrary::RemoveGameplayTagToActorIfNode(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
			UWarriorFunctionLibrary::RemoveGameplayTagToActorIfNode(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_None);
		}

		if (auto HerUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			HerUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		const FString DebugString = FString::Printf(
			TEXT("Old Health = %f, Damage = %f, NewCurrentHealth = %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth);
		Debug::Print(DebugString, FColor::Green);
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (GetCurrentHealth() == 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNode(
				Data.Target.GetAvatarActor(),
				WarriorGameplayTags::Shared_Status_Dead);
		}
	}
}
