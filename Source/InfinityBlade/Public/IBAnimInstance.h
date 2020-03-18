// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Animation/AnimInstance.h"
#include "IBAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIBAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	
	
	
};
