// Fill out your copyright notice in the Description page of Project Settings.

#include "IBAnimInstance.h"


UIBAnimInstance::UIBAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
}

void UIBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		//ABLOG(Warning, TEXT("///////// CurrentPawnSpeed: %d /////////"), CurrentPawnSpeed);
	}
}