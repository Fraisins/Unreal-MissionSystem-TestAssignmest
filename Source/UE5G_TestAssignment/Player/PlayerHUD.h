// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UMainHUDWidget;

UCLASS(Abstract, Blueprintable)
class UE5G_TESTASSIGNMENT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// ---- STATIC ----
	
	/** Try to return APlayerHUD from any actor. Returns nullptr if not found. */
	UFUNCTION(BlueprintPure, Category = "Controller")
	static APlayerHUD* GetPlayerHUDFromActor(AActor* Actor);
	
	// ---- GETTERS -----
	
	UFUNCTION(BlueprintPure, Category = "Controller|HUD")
	UMainHUDWidget* GetGameHUD() const { return GameHUD; }
	
	// ---- MINI GAMES ----
	
	void AddMiniGameToScreen(UUserWidget* MiniGameWidget); // shows widget and disables regular input
	void RemoveCurrentMiniGameFromScreen(); // removes widget and restores regular input
	
protected:
	// ---- BASIC FUNCTIONS ----
	virtual void BeginPlay() override;
	
	// ---- UI -----
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters|UI")
	TSubclassOf<UMainHUDWidget> MainHUDClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Parameters|UI")
	UUserWidget* CurrentMiniGameWidget;
	
private:
	// ---- UI -----
	
	UPROPERTY()
	UMainHUDWidget* GameHUD;
};
