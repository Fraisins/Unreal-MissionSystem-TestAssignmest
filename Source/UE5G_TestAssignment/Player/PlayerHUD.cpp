// Test assignment - Fraisins 2026


#include "PlayerHUD.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UE5G_TestAssignment/UI/MainHUDWidget.h"

// ---- BASIC FUNCTIONS ----

void APlayerHUD::BeginPlay()
{
	// create and add HUD
	GameHUD = CreateWidget<UMainHUDWidget>(GetOwningPlayerController(), MainHUDClass);
	GameHUD->AddToViewport();
}

// ---- MINI GAMES ----

void APlayerHUD::AddMiniGameToScreen(UUserWidget* MiniGameWidget)
{
	if (!MiniGameWidget) return;
	
	if (CurrentMiniGameWidget)
	{
		if (CurrentMiniGameWidget == MiniGameWidget)
			return;
		
		RemoveCurrentMiniGameFromScreen(); // remove if other minigame is on screen
	}
	
	CurrentMiniGameWidget = MiniGameWidget;
	CurrentMiniGameWidget->AddToViewport(1); // upper layer than main HUD
	
	// input to minigame widget
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;
	
	FInputModeUIOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
	
	// make character stop moving
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PC->GetPawn()))
	{
		PlayerCharacter->SetMovementEnabled(false);
	}
}

void APlayerHUD::RemoveCurrentMiniGameFromScreen()
{
	if (CurrentMiniGameWidget)
	{
		CurrentMiniGameWidget->RemoveFromParent();
		CurrentMiniGameWidget = nullptr;
	}
	
	// input to game
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;
	
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = false;
	
	// restore movement
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PC->GetPawn()))
	{
		PlayerCharacter->SetMovementEnabled(true);
	}
}

// ---- STATIC ----
APlayerHUD* APlayerHUD::GetPlayerHUDFromActor(AActor* Actor)
{
	if (!Actor) return nullptr;
	
	// if passed a controller
	if (APlayerController* PC = Cast<APlayerController>(Actor))
	{
		return Cast<APlayerHUD>(PC->GetHUD());
	}
	
	// if passed pawn
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
			return Cast<APlayerHUD>(PC->GetHUD());
	}
	
	// something else
	return nullptr;
}