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
	
protected:
	// ---- BASIC FUNCTIONS ----
	
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
	
private:
	void SetPitchRotationLimits(float InPitchMin, float InPitchMax);
};
