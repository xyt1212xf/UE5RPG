// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorEnumType.h"
#include "WarriorFunctionLibrary.generated.h"

class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;
struct FScalableFloat;
class UWarriorGameInstance;
/**
 * 
 */



UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNode(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayTagToActorIfNode(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfireType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfireType);

	static UPawnCombatComponent* NativeGetPawnCombatComponent(AActor* InActor);
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "GetPawnCombatComponent", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponent(AActor* InActor, EWarriorValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevle = 1.f);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);
	
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);


	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo="LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime,
		EWarriorCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& CountDownOutput,
		FLatentActionInfo LatentInfo);
	
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UWarriorGameInstance* GetWarriorGameInstance(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, EWarriroInputMode InputMode);
};
