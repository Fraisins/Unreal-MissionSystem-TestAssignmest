// Test assignment - Fraisins 2026


#include "MissionPointReachObjective.h"
#include "UE5G_TestAssignment/Triggers/CollisionTrigger.h"
#include "Misc/DataValidation.h"

// ---- CONSTRUCT ----

UMissionPointReachObjective::UMissionPointReachObjective()
{
	Description = FText::FromString("Reach the destination point.");
}

// ---- BASIC OVERRIDES ----

void UMissionPointReachObjective::ActivateObjective()
{
	Super::ActivateObjective(); // NECESSARY: changes state of objective
	
	ACollisionTrigger* Trigger = TargetTrigger.Get();
	
	if (!Trigger) // fail if trigger is null
	{
		UE_LOG(LogMissions, Warning, TEXT("[MissionPointReachObjective]: Error - TargetTrigger is null or not loaded!"));
		FailObjective();
		return;
	}
	
	// just cache this so GetObjectiveLocation() won't cast every time
	CachedTargetTrigger = Trigger;
	CachedTargetTrigger->OnActorEnterTrigger.AddDynamic(this, &UMissionPointReachObjective::OnColliderTriggered);
	CachedTargetTrigger->TryActivateTrigger();
}

void UMissionPointReachObjective::DeactivateObjective()
{
	if (CachedTargetTrigger)
	{
		CachedTargetTrigger->OnActorEnterTrigger.RemoveDynamic(this, &UMissionPointReachObjective::OnColliderTriggered);
		CachedTargetTrigger->TryDisableTrigger();
	}
	
	Super::DeactivateObjective();
}

// ---- GETTERS ----

AActor* UMissionPointReachObjective::GetActorTarget() const
{
	if (CachedTargetTrigger)
	{
		return CachedTargetTrigger;
	}
	return nullptr; 
}

// ---- TRIGGER ----

void UMissionPointReachObjective::OnColliderTriggered(ACollisionTrigger* Trigger, AActor* TriggeredActor)
{
	// could add some checks here, but instead left it to trigger to decide.
	CompleteObjective();
}

// --- VALIDATE ----

EDataValidationResult UMissionPointReachObjective::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
	if (TargetTrigger.IsNull())
	{
		Context.AddError(FText::FromString(FString::Printf(
			TEXT("[UMissionPointReachObjective] Error: '%s' has null target trigger!"), 
			*GetName())));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}

