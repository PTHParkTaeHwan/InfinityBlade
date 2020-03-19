// Fill out your copyright notice in the Description page of Project Settings.

#include "IBAnimInstance.h"
#include "IBCharacter.h"


UIBAnimInstance::UIBAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
}

void UIBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		//Pawn->GetMovementComponent()->IsFalling();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
		auto testCh = Cast<AIBCharacter>(Pawn);
		if (testCh)
		{			
			IsRun = testCh->GetIsRun();
		}

	}
}