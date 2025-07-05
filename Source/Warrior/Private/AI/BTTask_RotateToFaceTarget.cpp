// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToFaceTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Native Rotate To Face Target Actor");
	AnglePrecision = 10.f;
	RotationInterpSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	INIT_TASK_NODE_NOTIFY_FLAGS();
	InTargetFoFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetFoFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetFoFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString  KeyDescription = InTargetFoFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Smoothly rotates to face %s key until the angle precision: %s is reached"),
		*KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObjectPtr = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetFoFaceKey.SelectedKeyName);
	AActor* TargetActorPtr = Cast<AActor>(ActorObjectPtr);
	APawn* OwningPawnPtr = OwnerComp.GetAIOwner()->GetPawn();
	FRotateToFaceTargetTaskMemory* MemoryPtr = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(MemoryPtr);
	MemoryPtr->OwningPawn = OwningPawnPtr;
	MemoryPtr->TargetActor = TargetActorPtr;
	if (!MemoryPtr->IsValid())
	{
		return EBTNodeResult::Failed;
	}
	if (HasReachedAnglePercision(OwningPawnPtr,TargetActorPtr))
	{
		MemoryPtr->Reset();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToFaceTargetTaskMemory* MemoryPtr = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	if (!MemoryPtr->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	if (HasReachedAnglePercision(MemoryPtr->OwningPawn.Get(), MemoryPtr->TargetActor.Get()))
	{
		MemoryPtr->Reset(); 
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MemoryPtr->OwningPawn->GetActorLocation(), MemoryPtr->TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(MemoryPtr->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
		MemoryPtr->OwningPawn->SetActorRotation(TargetRot);
	}
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();
	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);
	return AngleDiff <= AnglePrecision;

}
