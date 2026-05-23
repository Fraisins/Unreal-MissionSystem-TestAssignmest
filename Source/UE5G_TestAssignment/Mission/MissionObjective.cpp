// Test assignment - Fraisins 2026


#include "MissionObjective.h"

// ---- OBJECTIVE ----

void UMissionObjective::ActivateObjective()
{
	ChangeState(EMissionState::Active);
}

void UMissionObjective::FailObjective()
{
	ChangeState(EMissionState::Failed);
	DeactivateObjective();
}

void UMissionObjective::CompleteObjective()
{
	ChangeState(EMissionState::Completed);
	DeactivateObjective();
}

void UMissionObjective::ForceComplete()
{
	CompleteObjective();
}

void UMissionObjective::ForceFail()
{
	FailObjective();
}

// ---- STATE ----

void UMissionObjective::ChangeState(EMissionState NewState)
{
	State = NewState;
	OnObjectiveStateChanged.Broadcast(this, State);
}
