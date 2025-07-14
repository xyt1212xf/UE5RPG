// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WarriorProjectileBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitysystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"


// Sets default values
AWarriorProjectileBase::AWarriorProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionBoxPtr = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBoxPtr"));	
	SetRootComponent(ProjectileCollisionBoxPtr);
	ProjectileCollisionBoxPtr->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBoxPtr->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBoxPtr->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBoxPtr->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileCollisionBoxPtr->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBoxPtr->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	ProjectileNiagaraComponentPtr = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponentPtr"));
	ProjectileNiagaraComponentPtr->SetupAttachment(GetRootComponent());

	ProjectileMovementComponentPro = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponentPro"));
	ProjectileMovementComponentPro->InitialSpeed = 700.f;
	ProjectileMovementComponentPro->MaxSpeed = 900.f;
	ProjectileMovementComponentPro->Velocity = FVector(1.f, 0.f, 0.f);
	ProjectileMovementComponentPro->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

// Called when the game starts or when spawned
void AWarriorProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (EProjectileDamagePolicy::OnBeginOverlap == ProjectileDamagePolicy)
	{
		ProjectileCollisionBoxPtr->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AWarriorProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn || !UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
	{
		Destroy();
		return;
	}
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitPawn, WarriorGameplayTags::Player_Status_Blocking);
	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(this, HitPawn);
	}
	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			Data);
	}
	else
	{
		HandleApplyProjectileDamage(HitPawn, Data);
	}
	Destroy();
}

void AWarriorProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AWarriorProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
	check(ProjectileDamageEffectSpecHandle.IsValid());
	const bool bWasApplied = UWarriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);
	if (bWasApplied)
	{
		FGameplayEventData Data;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitPawn,
			WarriorGameplayTags::Shared_Event_HitReact,
			InPayload);
	}
}



