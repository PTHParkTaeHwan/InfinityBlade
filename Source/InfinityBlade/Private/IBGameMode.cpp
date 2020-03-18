// Fill out your copyright notice in the Description page of Project Settings.

#include "IBGameMode.h"
#include "IBPawn.h"
#include "IBCharacter.h"
#include "IBPlayerController.h"

AIBGameMode::AIBGameMode()
{
	DefaultPawnClass = AIBCharacter::StaticClass();
	PlayerControllerClass = AIBPlayerController::StaticClass();
}


