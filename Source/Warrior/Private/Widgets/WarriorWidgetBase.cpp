// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WarriorWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (auto PawnUIIterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (auto* HeroUIComponent = PawnUIIterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}

void UWarriorWidgetBase::InitEnemyCreateWidgets(AActor* OwningEnemyActor)
{
	if (auto PawnUIIterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		if (auto* EnemyUIComponent = PawnUIIterface->GetEnemyUIComponent())
		{
			checkf(EnemyUIComponent, TEXT("ERROR-- > 003"));
			BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
		}
	}
}
