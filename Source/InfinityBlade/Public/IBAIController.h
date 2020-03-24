// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "AIController.h"
#include "IBAIController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API AIBAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AIBAIController();
	virtual void Possess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;
	
	UPROPERTY()
	class UBlackboardData* BBAsset;
	
};
