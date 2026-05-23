// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "MissionDataAsset.h"
#include "MissionTypes.h"
#include "UObject/Object.h"
#include "MissionInstance.generated.h"

class UMissionDataAsset;
class UMissionObjective;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionStateChanged, UMissionInstance*, MissionInstance, 
	EMissionState, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentObjectiveChanged, UMissionInstance*, MissionInstance, 
	UMissionObjective*, Objective);

/**
 * Runtime instance of a mission created from UMissionDataAsset.
 * Manages objectives progression and notifies listeners via delegates.
 * Created and owned by UMissionSubsystem.
 */
UCLASS()
class UE5G_TESTASSIGNMENT_API UMissionInstance : public UObject
{
	GENERATED_BODY()
	
public:
	// ---- INIT ----
	
	bool Initialize(const UMissionDataAsset* InSourceAsset); // to return value to subsystem if initialization failed
	
	// ---- ACTIVATE / FAIL / COMPLETE ----
	
	/** Activates mission and starts it's objective. */
	UFUNCTION(BlueprintCallable, Category="Mission")
	void ActivateMission();
	
	/** Forces mission into completed state. 
	 * Normally called internally when all objectives are done. */
	UFUNCTION(BlueprintCallable, Category="Mission")
	void ForceComplete();
	
	/** Forces mission into completed state. 
	 * Normally called internally when mission encountered error, or any objective failed. */
	UFUNCTION(BlueprintCallable, Category="Mission")
	void ForceFail();
	
	// ---- DELEGATES ----
	
	UPROPERTY(BlueprintAssignable, Category = "Mission|Delegates")
	FOnMissionStateChanged OnMissionStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Mission|Delegates")
	FOnCurrentObjectiveChanged OnCurrentObjectiveChanged;
	
	// ---- GETTERS ----
	
	/** Get Source Data Asset of this mission  */
	UFUNCTION(BlueprintPure, Category="Mission|Getters")
	const UMissionDataAsset* GetSourceAsset() const { return SourceAsset; }
	
	/** Get all runtime mission objectives  */
	UFUNCTION(BlueprintPure, Category="Mission|Getters")
	const TArray<UMissionObjective*>& GetRuntimeObjectives() const { return RuntimeObjectives; }
	
	/** Get current objective  */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	UMissionObjective* GetCurrentObjective() const { return RuntimeObjectives[CurrentObjectiveIndex]; }
	
	/** Get current mission state  */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	EMissionState GetMissionState() const { return MissionState; }
	
	/** Get mission ID  */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FName GetMissionId() const;
	
	/** Get mission FText name (FOR UI) */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FText GetMissionPublicName() const { return SourceAsset ? 
		SourceAsset->MissionPublicName : 
		FText::GetEmpty(); }
	
	/** Get mission description from its current objective */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FText GetMissionDescription() const { return GetCurrentObjective() ? 
		GetCurrentObjective()->GetDescription() : 
		FText::GetEmpty(); }
	
	/** Get mission indicator class (taken from UMissionDataAsset) */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	TSubclassOf<AObjectiveIndicator> GetObjectiveIndicatorClass() const { return SourceAsset ? 
		SourceAsset->ObjectiveIndicatorClass : 
		nullptr; }
	
private:
	
	void FailMission();
	void CompleteMission();
	
	// ---- SOURCE ----
	
	UPROPERTY()
	const UMissionDataAsset* SourceAsset;
	
	// ---- STATE ----
	
	EMissionState MissionState = EMissionState::Inactive;
	
	void ChangeMissionState(EMissionState NewState);
	
	// ---- OBJECTIVES AND PROGRESS ----
	
	UPROPERTY()
	TArray<UMissionObjective*> RuntimeObjectives;
	
	UPROPERTY()
	uint8 CurrentObjectiveIndex = -1; // -1 because it will turn to 0 on activation
	// uint8 because 255 objectives sound like enough...
	
	// For objective delegate
	UFUNCTION()
	void OnObjectiveStateChanged(UMissionObjective* Objective, EMissionState NewState);
	
	bool ActivateNextObjective();
	bool IsMissionComplete() const;
	void CheckAndProgressMission(); // Checks mission completeness and progresses it
	
	bool bShouldSkipProgress; // to stop progress and not call next objective
};
