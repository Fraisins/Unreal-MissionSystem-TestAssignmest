// Test assignment - Fraisins 2026


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UE5G_TestAssignment/UI/MainHUDWidget.h"

// ---- BASIC FUNCTIONS ----

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetPitchRotationLimits(PitchMin, PitchMax);
}

// ---- INPUT ----

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

// ---- ROTATION ---

void AMainPlayerController::SetPitchRotationLimits(float InPitchMin, float InPitchMax)
{
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = InPitchMin;
		PlayerCameraManager->ViewPitchMax = InPitchMax;
	}
	else
	{
		UE_LOG(LogPlayerController, Error, TEXT("[%s] Failed to set pitch limits - PlayerCameraManager is null"), 
			*GetNameSafe(this));
	}
}