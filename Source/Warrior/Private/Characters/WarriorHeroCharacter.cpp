// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WarriorGameplayTags.h"
#include "Components/Input/WarriorInputComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorDebugHelper.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoomPtr = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomPtr"));
	CameraBoomPtr->SetupAttachment(GetRootComponent());
	CameraBoomPtr->TargetArmLength = 200.f;
	CameraBoomPtr->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoomPtr->bUsePawnControlRotation = true;

	FollowCameraPtr = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCameraPtr"));
	FollowCameraPtr->SetupAttachment(CameraBoomPtr, USpringArmComponent::SocketName);
	FollowCameraPtr->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponentPtr = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponentPtr"));
	
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AWarriorHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponentPtr;
}

UPawnUIComponent* AWarriorHeroCharacter::GetPawnUIComponent() const
{
	return GetHeroUIComponent();
}

UHeroUIComponent* AWarriorHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponentPtr);
		}
	}
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAssetPtr->DefaultMappingContextPtr, 0);
	auto* WarriorInoutComonent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);
	WarriorInoutComonent->BindNativeInputAction(InputConfigDataAssetPtr, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this,&ThisClass::Input_Move);
	WarriorInoutComonent->BindNativeInputAction(InputConfigDataAssetPtr, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this,&ThisClass::Input_Look);
	
	WarriorInoutComonent->BindNativeInputAction(InputConfigDataAssetPtr, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this,&ThisClass::Input_SwitchTargetTriggered);
	WarriorInoutComonent->BindNativeInputAction(InputConfigDataAssetPtr, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this,&ThisClass::Input_SwitchTargetCompleted);
	
	WarriorInoutComonent->BindAbilityInputAction(InputConfigDataAssetPtr, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);

}

void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	auto MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	if (MovementVector.Y != 0)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (MovementVector.X != 0)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	auto LookAxisVector = InputActionValue.Get<FVector2D>();
	if (LookAxisVector.X != 0)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWarriorHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AWarriorHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.f ? WarriorGameplayTags::Player_Event_SwitchTarget_Left : WarriorGameplayTags::Player_Event_SwitchTarget_Right,
		Data);
}

void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponentPtr->OnAbilityInputPressed(InInputTag);
}

void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponentPtr->OnAbilityInputReleased(InInputTag);
}
