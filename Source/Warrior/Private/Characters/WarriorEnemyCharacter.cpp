// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/WidgetComponent.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Components/BoxComponent.h"
#include "WarriorFunctionLibrary.h"
#include "GameModes/WarriorBaseGameMode.h"
#include "WarriorDebugHelper.h"


AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	//表示这个敌人角色在 被放置到世界中 或 被生成时 会自动被 AI 控制器接管
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//禁止角色使用控制器的旋转值来控制自身的朝向（Pitch、Roll、Yaw）。通常用于让角色 根据移动方向自动转向，而不是硬绑定控制器朝向
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//角色不会强行朝向控制器方向。
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//角色会 根据移动方向自动旋转。
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//旋转速度为每秒 180 度（Yaw 轴）。
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	//最大行走速度为 300 单位 / 秒。
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	//行走时的减速加速度为 1000，让角色停下更快。
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponentPtr = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponentPtr"));
	
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));
	
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));

	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

	LeftHandCollisionBoxPtr = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBoxPtr"));
	LeftHandCollisionBoxPtr->SetupAttachment(GetMesh());
	LeftHandCollisionBoxPtr->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBoxPtr->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBoxPtr = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBoxPtr"));
	RightHandCollisionBoxPtr->SetupAttachment(GetMesh());
	RightHandCollisionBoxPtr->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBoxPtr->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
}

UPawnCombatComponent* AWarriorEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponentPtr;
}

UPawnUIComponent* AWarriorEnemyCharacter::GetPawnUIComponent() const
{
	return GetEnemyUIComponent();
}

UEnemyUIComponent* AWarriorEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AWarriorEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UWarriorWidgetBase* HealthWidget = Cast<UWarriorWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreateWidgets(this);
	}
}

void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitEnemyStartUpData();

}

void AWarriorEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UWarriorFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			EnemyCombatComponentPtr->OnHitTargetActor(HitPawn);
		}
	}
}

#if WITH_EDITOR
void AWarriorEnemyCharacter::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBoxPtr->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBoxPtr->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
} 
#endif

void AWarriorEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}
	int32 AbilityApplyLevel = 1;
	if (AWarriorBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AWarriorBaseGameMode>())
	{
		switch (BaseGameMode->GetCurrentWarriroGameDifficult())
		{
		case EWarriorGameDifficult::Easy:
			AbilityApplyLevel = 1;
		case EWarriorGameDifficult::Normal:
			AbilityApplyLevel = 2;
		case EWarriorGameDifficult::Hard:
			AbilityApplyLevel = 3;
		case EWarriorGameDifficult::VeryHard:
			AbilityApplyLevel = 4;
		default:
			break;
		}
	}
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, AbilityApplyLevel]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponentPtr, AbilityApplyLevel);
				}
			}
		)
	);
}
