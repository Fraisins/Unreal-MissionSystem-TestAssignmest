// Test assignment - Fraisins 2026


#include "MissionInstance.h"
#include "MissionDataAsset.h"
#include "MissionObjective.h"

// ---- INIT ----

bool UMissionInstance::Initialize(const UMissionDataAsset* InSourceAsset)
{
	if (!InSourceAsset)
	{
		UE_LOG(LogMissions, Error, TEXT("[UMissionInstance] failed to initialize : InSourceAsset is null!"));
		return false;
	}
	
	SourceAsset = InSourceAsset;
	
	for (UMissionObjective* Objective : SourceAsset->MissionObjectives)
	{
		if (!Objective) continue;
		
		// runtime copies to not touch DataAsset's objectives
		UMissionObjective* RuntimeObjectiveCopy = DuplicateObject<UMissionObjective>(Objective, this);
		RuntimeObjectives.Add(RuntimeObjectiveCopy);
		// we'll subscribe later, before activating objective
	}
	return true;
}

// ---- ACTIVATE / FAIL / COMPLETE ----

void UMissionInstance::ActivateMission()
{
	if (!ActivateNextObjective()) // try to activate first objective (at 0 index)
	{
		FailMission(); // fail mission if something goes wrong
		return;
	}
	
	ChangeMissionState(EMissionState::Active);
}

void UMissionInstance::FailMission()
{
	ChangeMissionState(EMissionState::Failed);
}

void UMissionInstance::CompleteMission()
{
	ChangeMissionState(EMissionState::Completed);
}

void UMissionInstance::ForceComplete()
{
	bShouldSkipProgress = true; // set before to stop progress continue next objectives
	
	UMissionObjective* CurrentObjective = GetCurrentObjective();
	if (!CurrentObjective) return;
	CurrentObjective->ForceComplete();
	
	CompleteMission();
}

void UMissionInstance::ForceFail()
{
	bShouldSkipProgress = true; // set before to stop progress continue next objectives
	
	UMissionObjective* CurrentObjective = GetCurrentObjective();
	if (!CurrentObjective) return;
	CurrentObjective->ForceFail();
	
	FailMission();
}

// ---- STATE ----

void UMissionInstance::ChangeMissionState(EMissionState NewState)
{
	MissionState = NewState;
	OnMissionStateChanged.Broadcast(this, MissionState);
}

// ---- OBJECTIVES AND PROGRESS ----

void UMissionInstance::OnObjectiveStateChanged(UMissionObjective* Objective, EMissionState NewState)
{
	if (GetCurrentObjective() != Objective)
		return;
	
	switch (NewState)
	{
	case EMissionState::Failed:
		Objective->OnObjectiveStateChanged.RemoveAll(this);
		FailMission();
		return;
	case EMissionState::Completed:
		Objective->OnObjectiveStateChanged.RemoveAll(this);
		CheckAndProgressMission();
		return;
	default:
		break;
	}
}

bool UMissionInstance::ActivateNextObjective()
{
	CurrentObjectiveIndex++;
	
	// check if new index valid
	if (!RuntimeObjectives.IsValidIndex(CurrentObjectiveIndex))
		return false; // fail mission if not
	
	UMissionObjective* Objective = RuntimeObjectives[CurrentObjectiveIndex];
	
	if (!Objective) // null check
	{
		UE_LOG(LogMissions, Error, TEXT("[UMissionInstance] Trying to activate null objective!"));
		return false;
	}
	Objective->OnObjectiveStateChanged.AddUObject(this, &UMissionInstance::OnObjectiveStateChanged);
	Objective->ActivateObjective();
	
	OnCurrentObjectiveChanged.Broadcast(this, Objective); // broadcast after activation
	
	return true;
}

bool UMissionInstance::IsMissionComplete() const
{
	UMissionObjective* CurrentObjective = GetCurrentObjective();
	if (!CurrentObjective)  // validation in data asset shouldn't theoretically allow this
		return false;		// but i decided to add it just in case
	return CurrentObjectiveIndex == RuntimeObjectives.Num() - 1 // is last mission?
		&& CurrentObjective->GetState() == EMissionState::Completed; // is last objective completed?
}

void UMissionInstance::CheckAndProgressMission()
{
	if (bShouldSkipProgress)
		return;
	
	if (IsMissionComplete()) // check if complete
	{
		CompleteMission();
		return;
	}
	ActivateNextObjective(); // activate next objective
}