// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionListWidget.generated.h"

class USizeBox;
class UMissionSubsystem;
class UMissionEntryWidget;
class UVerticalBox;

/**
 * Widget displaying a list of active missions.
 * Subscribes to UMissionSubsystem delegates and updates entries automatically.
 */
UCLASS(Abstract)
class UE5G_TESTASSIGNMENT_API UMissionListWidget : public UUserWidget
{
	GENERATED_BODY()
	
	// ---- BASE FUNCTIONS ----
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	// --- BIND WIDGETS ----
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* MissionList;
	
	// ---- MISSIONS ----
	
	/** Mission entry widget class. */
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	TSubclassOf<UMissionEntryWidget> MissionEntryClass;
	
	/** How many missions entries o show */
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	int MaxEntryCount;
	
	/** Bottom padding for spacing mission entries */
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	float MissionsEntryPaddingBottom;
	
	/** Updates entries depending on current missions in subsystem and IDArrayOrder*/
	void UpdateEntries();
	
private:
	// ---- MISSIONS ----
	
	UPROPERTY()
	UMissionSubsystem* MissionSubsystem;
	
	UPROPERTY()
	TArray<UMissionEntryWidget*> MissionEntries;
	
	UPROPERTY()
	TArray<FName> IDArrayOrder;
	
	UFUNCTION()
	void MissionRemoved(FName MissionID);
	
	UFUNCTION()
	void MissionUpdated(FName MissionID);
};
