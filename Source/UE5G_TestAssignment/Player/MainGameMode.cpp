// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameMode.h"

#include "MainPlayerController.h"
#include "PlayerCharacter.h"

AMainGameMode::AMainGameMode()
{
	// just default values for now...
	
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
