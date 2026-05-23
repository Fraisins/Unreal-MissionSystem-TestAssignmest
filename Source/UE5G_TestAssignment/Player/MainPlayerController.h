// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UMainHUDWidget;
class UInputMappingContext;

UCLASS(Abstract, Blueprintable)
class UE5G_TESTASSIGNMENT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// ---- GETTERS -----
	
	UFUNCTION(BlueprintPure, Category = "Controller|HUD")
	UMainHUDWidget* GetGameHUD() const { return GameHUD; }
	
	// ---- MINI GAMES ----
	
	void AddMiniGameToScreen(UUserWidget* MiniGameWidget); // shows widget and disables regular input
	void RemoveCurrentMiniGameFromScreen(); // removes widget and restores regular input
	
	// ---- STATIC ----
	
	/** Try to return MainPlayerController from any actor. Returns nullptr if not found. */
	UFUNCTION(BlueprintPure, Category = "Controller")
	static AMainPlayerController* GetMainPlayerControllerFromActor(AActor* Actor);
	
protected:
	// ---- BASIC FUNCTIONS ----
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;  // pitch limits set here
	
	// ---- INPUT ----
	
	UPROPERTY(EditDefaultsOnly, Category = "Parameters|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	virtual void SetupInputComponent() override;
	
	// ---- ROTATION ----
	
	UPROPERTY(EditDefaultsOnly, Category = "Parameters|Rotation")
	float PitchMin = -60.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Parameters|Rotation")
	float PitchMax = 60.f;
	
	void SetPitchRotationLimits(float InPitchMin, float InPitchMax);
	
	// ---- UI -----
	
	UPROPERTY(EditDefaultsOnly, Category = "Parameters|UI")
	TSubclassOf<UMainHUDWidget> MainHUDClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Parameters|UI")
	UUserWidget* CurrentMiniGameWidget;
	
private:
	// ---- UI -----
	
	UPROPERTY()
	UMainHUDWidget* GameHUD;
};
