// Fill out your copyright notice in the Description page of Project Settings.

#include "IBPlayerController.h"

void AIBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}


