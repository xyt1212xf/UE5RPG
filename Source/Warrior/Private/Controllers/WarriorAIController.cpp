// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WarriorDebugHelper.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{

	AISceneConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySceneConfig_Sight"));
	//确认敌人，忽略队友和中立
	AISceneConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISceneConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISceneConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	// 5 米内进入视野
	AISceneConfig_Sight->SightRadius = 5000.f;
	// 丢失半径 0 → 一旦看到就永远看得到（需手动清除）
	AISceneConfig_Sight->LoseSightRadius = 0.f;
	// 360° 全向，没有背后盲区
	AISceneConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*AISceneConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);
	SetGenericTeamId(FGenericTeamId(1));

}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		//SetCrowdSimulationState 就是“即时开关群体算法”的总闸，随时让 AI 在“聪明避让”与“传统寻路”之间切换
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);break;	
		case 2:	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);break;	
		case 3:	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);break;	
		case 4:	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);break;			
		}
		
		// 我自己属于 Group 1
		CrowdComp->SetAvoidanceGroup(1);
		// 我只避开 Group 1（即同组也互让）
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}

