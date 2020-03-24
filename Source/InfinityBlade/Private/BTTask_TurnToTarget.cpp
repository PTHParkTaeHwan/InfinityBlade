// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "IBAIController.h"
#include "IBCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto IBCharacter = Cast<AIBCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == IBCharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<AIBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIBAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - IBCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	IBCharacter->SetActorRotation(FMath::RInterpTo(IBCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
