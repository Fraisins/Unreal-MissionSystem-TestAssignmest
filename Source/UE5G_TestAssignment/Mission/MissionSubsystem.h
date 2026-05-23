// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "MissionTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MissionSubsystem.generated.h"

class UMissionSettings;
class AObjectiveIndicator;
class UMissionObjective;
class UMissionInstance;
class UMissionDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMission, FName, MissionID);

/**
 * Subsystem that manages all game missions lifecycle.
 * Handles starting, completing, failing missions and notifies listeners via delegates.
 * Accessible from anywhere via UGameInstance::GetSubsystem<UMissionSubsystem>().
 */
UCLASS()
class UE5G_TESTASSIGNMENT_API UMissionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// ---- MISSIONS ----
	
	/**
	 * Starts a mission from the given data asset and registers it as active.
	 * @param MissionData  Asset with mission configuration.
	 * @return true if mission started successfully; false if already active or data is invalid. */
	UFUNCTION(BlueprintCallable, Category="Missions")
	bool StartMission(UMissionDataAsset* MissionData);
	
	/**
	 * Forces mission completion from outside (for cheats, debug, manual settings, etc...)
	 * Normally missions complete themselves via their own logic.
	 * @param MissionID Primary asset ID of the mission to complete. */
	UFUNCTION(BlueprintCallable, Category="Missions")
	void ForceCompleteMission(FName MissionID);
	
	/**
	 * Forces mission failure from outside (for cheats, debug, manual settings, etc...)
	 * Normally missions complete themselves via their own logic.
	 * @param MissionID  Primary asset ID of the mission to fail. */
	UFUNCTION(BlueprintCallable, Category="Missions")
	void ForceFailMission(FName MissionID);
	
	// ---- GETTERS ----
	
	/** Returns all currently active missions. */
	UFUNCTION(BlueprintPure, Category="Missions|Getters")
	const TMap<FName, UMissionInstance*>& GetActiveMissions() const { return ActiveMissions; }
	
	/** Returns the mission instance for the given ID, or nullptr if not active. */
	UFUNCTION(BlueprintPure, Category="Missions|Getters")
	UMissionInstance* GetMissionInstance(FName MissionID) const
		{ return ActiveMissions.Contains(MissionID) ? ActiveMissions[MissionID] : nullptr; }
	
	// ---- DELEGATES ----
	
	/** Fired when a new mission is started. */
	UPROPERTY(BlueprintAssignable, Category="Missions|Delegates")
	FOnMission OnMissionAdded;
	
	/** Fired when am active mission completed successfully. */
	UPROPERTY(BlueprintAssignable, Category="Missions|Delegates")
	FOnMission OnMissionCompleted;
	
	/** Fired when am active mission failed */
	UPROPERTY(BlueprintAssignable, Category="Missions|Delegates")
	FOnMission OnMissionFailed;
	
	/** Fired when mission changes (like objective updated). */
	UPROPERTY(BlueprintAssignable, Category="Missions|Delegates")
	FOnMission OnMissionUpdated;
	
private:
	// ---- MISSIONS ----
	
	UPROPERTY()
	TMap<FName, UMissionInstance*> ActiveMissions;
	
	void CompleteMission(FName MissionID);
	
	void FailMission(FName MissionID);
	
	bool DeactivateMission(FName MissionID);
	
	UFUNCTION() // for delegate catching mission state changing
	void OnMissionStateChanged(UMissionInstance* MissionInstance, EMissionState NewState);
	
	UFUNCTION() // for delegate catching mission objective changing
	void OnMissionObjectiveChanged(UMissionInstance* MissionInstance, UMissionObjective* NewObjective);
	
	// ---- OBJECTIVE ----
	
	// handles spawning indicator for objective (if objective supports it)
	AObjectiveIndicator* SpawnObjectiveIndicator(UMissionInstance* MissionInstance);
};
