// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "MissionDataAsset.h"
#include "MissionTypes.h"
#include "UObject/Object.h"
#include "MissionInstance.generated.h"

class UMissionInstance;
class UMissionDataAsset;
class UMissionObjective;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMissionStateChanged, UMissionInstance* MissionInstance, 
	EMissionState State);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentObjectiveChanged, UMissionInstance* MissionInstance, 
	UMissionObjective* Objective);

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
	
	FOnMissionStateChanged OnMissionStateChanged;
	FOnCurrentObjectiveChanged OnCurrentObjectiveChanged;
	
	// ---- GETTERS ----
	
	/** Get all runtime mission objectives  */
	UFUNCTION(BlueprintPure, Category="Mission|Getters")
	FORCEINLINE TArray<UMissionObjective*> GetRuntimeObjectives() const { return RuntimeObjectives; }
	
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FORCEINLINE UMissionObjective* GetCurrentObjective() const { 
		return RuntimeObjectives.IsValidIndex(CurrentObjectiveIndex) 
		? RuntimeObjectives[CurrentObjectiveIndex] 
		: nullptr; }
	
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FORCEINLINE EMissionState GetMissionState() const { return MissionState; }
	
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FORCEINLINE FName GetMissionId() const { return SourceAsset ?
		SourceAsset->MissionID :
		FName(""); }
	
	/** Get mission FText name (FOR UI) */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FORCEINLINE FText GetMissionPublicName() const { return SourceAsset ? 
		SourceAsset->MissionPublicName : 
		FText::GetEmpty(); }
	
	/** Get mission description from its current objective */
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FORCEINLINE FText GetMissionDescription() const { return GetCurrentObjective() ? 
		GetCurrentObjective()->GetDescription() : 
		FText::GetEmpty(); }
	
	UFUNCTION(BlueprintPure, Category = "Mission|Getters")
	FORCEINLINE TSubclassOf<AObjectiveIndicator> GetObjectiveIndicatorClass() const { return SourceAsset ? 
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
