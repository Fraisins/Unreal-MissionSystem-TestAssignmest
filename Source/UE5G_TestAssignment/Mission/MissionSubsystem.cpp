// Test assignment - Fraisins 2026


#include "MissionSubsystem.h"
#include "MissionDataAsset.h"
#include "MissionInstance.h"
#include "ObjectiveIndicator/ObjectiveIndicator.h"

// ---- MISSIONS ----

bool UMissionSubsystem::StartMission(UMissionDataAsset* MissionData)
{
	if (MissionData == nullptr)
		return false;
	
	UMissionInstance* NewMission = NewObject<UMissionInstance>(this);
	if (!NewMission->Initialize(MissionData)) // try initialize
		return false;
	
	const FName MissionID = MissionData->MissionID;
	
	// Missions handle their own state - subsystem subscribes to catch mission changes and broadcast own delegates
	NewMission->OnMissionStateChanged.AddUObject(this, &UMissionSubsystem::OnMissionStateChanged);
	NewMission->OnCurrentObjectiveChanged.AddUObject(this, &UMissionSubsystem::OnMissionObjectiveChanged);
	
	ActiveMissions.Add(MissionID, NewMission);
	NewMission->ActivateMission();
	
	OnMissionAdded.Broadcast(MissionID);
	return true;
}

void UMissionSubsystem::ForceCompleteMission(FName MissionID)
{
	UMissionInstance* Mission = GetMissionInstance(MissionID);
	if (Mission)
		Mission->ForceComplete();
}

void UMissionSubsystem::ForceFailMission(FName MissionID)
{
	UMissionInstance* Mission = GetMissionInstance(MissionID);
	if (Mission)
		Mission->ForceFail();
}

void UMissionSubsystem::CompleteMission(FName MissionID)
{
	if (!DeactivateMission(MissionID))
		return;
	OnMissionCompleted.Broadcast(MissionID);
}

void UMissionSubsystem::FailMission(FName MissionID)
{
	if (!DeactivateMission(MissionID))
		return;
	OnMissionFailed.Broadcast(MissionID);
}

bool UMissionSubsystem::DeactivateMission(FName MissionID)
{
	if (!ActiveMissions.Contains(MissionID))
		return false;
	
	UMissionInstance* Mission = ActiveMissions[MissionID];
	
	Mission->OnMissionStateChanged.RemoveAll(this);
	Mission->OnCurrentObjectiveChanged.RemoveAll(this);
	
	ActiveMissions.Remove(MissionID);
	
	return true;
}

void UMissionSubsystem::OnMissionStateChanged(UMissionInstance* MissionInstance, EMissionState NewState)
{
	FName MissionID = MissionInstance->GetMissionId();
	
	switch (NewState)
	{
	case EMissionState::Failed:
		FailMission(MissionID);
		break;
	case EMissionState::Completed:
		CompleteMission(MissionID);
		break;
	default:
		break;
	}
}

void UMissionSubsystem::OnMissionObjectiveChanged(UMissionInstance* MissionInstance, UMissionObjective* NewObjective)
{
	FName MissionID = MissionInstance->GetMissionId();
	
	if (NewObjective)
	{
		AActor* ObjectiveTarget = NewObjective->GetActorTarget();
		if (!IsValid(ObjectiveTarget))
		{
			UE_LOG(LogMissions, Warning, TEXT("ObjectiveTarget is invalid for mission %s"), 
				*MissionInstance->GetMissionId().ToString());
		}
		else
		{
			AObjectiveIndicator* Indicator = SpawnObjectiveIndicator(MissionInstance);
			if (Indicator)
			{
				Indicator->SetParent(ObjectiveTarget);
				Indicator->Activate(NewObjective);
			}
		}
	}
	
	OnMissionUpdated.Broadcast(MissionID);
}

// ---- OBJECTIVE

AObjectiveIndicator* UMissionSubsystem::SpawnObjectiveIndicator(UMissionInstance* MissionInstance)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;
	
	TSubclassOf<AObjectiveIndicator> IndicatorClass = MissionInstance->GetObjectiveIndicatorClass();
	if (!IndicatorClass) return nullptr;
	
	AObjectiveIndicator* Indicator = World->SpawnActor<AObjectiveIndicator>(
	IndicatorClass,
	FVector::ZeroVector,
	FRotator::ZeroRotator);
	
	return Indicator;
}


