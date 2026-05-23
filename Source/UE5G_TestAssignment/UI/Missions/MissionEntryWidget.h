// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionEntryWidget.generated.h"

class UTextBlock;
class UMissionInstance;

/**
 * Entry mission widget for UMissionListWidget
 */
UCLASS(Abstract)
class UE5G_TESTASSIGNMENT_API UMissionEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ---- BASIC FUNCTIONS ----
	
	virtual void NativeConstruct() override;
	
	// ---- MISSIONS ----
	
	void SetMissionInstance(UMissionInstance* MissionInstance);
	void ClearEntry();
	void SetMissionInfo(UMissionInstance* MissionInstance);
	
	// in Blueprint... why not
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionAppear();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionDisappear();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionChangedDescription();
	
protected:
	// --- BIND WIDGETS ----
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* MissionNameTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* DescriptionTextBlock;
	
private:
	bool bHasMissionEntry = false;
	
	UPROPERTY()
	UMissionInstance* CurrentMission;
};
