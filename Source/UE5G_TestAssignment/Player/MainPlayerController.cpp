// Test assignment - Fraisins 2026


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UE5G_TestAssignment/UI/MainHUDWidget.h"

// ---- BASIC FUNCTIONS ----

void AMainPlayerController::BeginPlay()
{
	// create and add HUD
	GameHUD = CreateWidget<UMainHUDWidget>(this, MainHUDClass);
	GameHUD->AddToViewport();
}

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
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to set pitch limits - PlayerCameraManager is null"), 
			*GetNameSafe(this));
	}
}

// ---- MINI GAMES ----

void AMainPlayerController::AddMiniGameToScreen(UUserWidget* MiniGameWidget)
{
	if (CurrentMiniGameWidget)
	{
		if (CurrentMiniGameWidget == MiniGameWidget)
			return;
		
		RemoveCurrentMiniGameFromScreen(); // remove if other minigame is on screen
	}
	
	CurrentMiniGameWidget = MiniGameWidget;
	CurrentMiniGameWidget->AddToViewport(1); // upper layer than main HUD
	
	// input to minigame widget
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
	
	// make character stop moving
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->SetMovementEnabled(false);
	}
}

void AMainPlayerController::RemoveCurrentMiniGameFromScreen()
{
	if (CurrentMiniGameWidget)
	{
		CurrentMiniGameWidget->RemoveFromParent();
		CurrentMiniGameWidget = nullptr;
	}
	
	// input to game
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	
	// restore movement
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->SetMovementEnabled(true);
	}
}

// ---- STATIC ----

AMainPlayerController* AMainPlayerController::GetMainPlayerControllerFromActor(AActor* Actor)
{
	if (APawn* PawnInstigator = Cast<APawn>(Actor))
	{
		return Cast<AMainPlayerController>(PawnInstigator->GetController());
	}
	return nullptr;
}
