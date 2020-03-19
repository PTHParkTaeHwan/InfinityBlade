// Fill out your copyright notice in the Description page of Project Settings.

#include "IBAnimInstance.h"
#include "IBCharacter.h"


UIBAnimInstance::UIBAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
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

void UIBAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);	
}

void UIBAnimInstance::AnimNotify_AttackHitCheck()
{
}
