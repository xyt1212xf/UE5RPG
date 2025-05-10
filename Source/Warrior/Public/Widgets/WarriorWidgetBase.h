// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarriorWidgetBase.generated.h"

class UHeroUIComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Hero UI ComponentInitialized"))
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* OwningHeroUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Enemy UI ComponentInitialized"))
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreateWidgets(AActor* OwningEnemyActor);

};
