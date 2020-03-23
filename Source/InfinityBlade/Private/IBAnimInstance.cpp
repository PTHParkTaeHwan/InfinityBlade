// Fill out your copyright notice in the Description page of Project Settings.

#include "IBAnimInstance.h"
#include "IBCharacter.h"


UIBAnimInstance::UIBAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	IsDefense = false;
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
	
	if (!::IsValid(Pawn)) return;
	

	if (!IsDead)
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
			IsDefense = testCh->GetIsDefense();
		}
	}
}

void UIBAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);	
}

void UIBAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UIBAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UIBAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UIBAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
