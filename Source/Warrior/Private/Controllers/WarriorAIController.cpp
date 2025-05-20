// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "WarriorDebugHelper.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("AWarriorAIController"));
	}
	AISceneConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySceneConfig_Sight"));
	AISceneConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISceneConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISceneConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISceneConfig_Sight->SightRadius = 5000.f;
	AISceneConfig_Sight->LoseSightRadius = 0.f;
	AISceneConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*AISceneConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

}
