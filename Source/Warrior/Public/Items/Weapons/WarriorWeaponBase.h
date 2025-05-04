// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorWeaponBase.generated.h"
class UBoxComponent;
DECLARE_DELEGATE_OneParam(FOnTargetInterfacedDelegate, AActor*)

UCLASS()
class WARRIOR_API AWarriorWeaponBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AWarriorWeaponBase();
	
	FOnTargetInterfacedDelegate OnWeaponHitTarget;
	FOnTargetInterfacedDelegate OnWeaponPulledFromTarget;

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* WeaponMeshPtr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBoxPtr;

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBoxPtr() const
	{
		return WeaponCollisionBoxPtr;
	}
};
