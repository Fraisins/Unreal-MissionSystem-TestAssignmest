// Test assignment - Fraisins 2026


#include "MissionInteractiveObjectObjective.h"
#include "UE5G_TestAssignment/Interaction/Interactable.h"
#include "Misc/DataValidation.h"

// ---- CONSTRUCT ----

UMissionInteractiveObjectObjective::UMissionInteractiveObjectObjective()
{
	Description = FText::FromString(TEXT("Interact with object"));
}

// ---- BASIC OVERRIDES ----

void UMissionInteractiveObjectObjective::ActivateObjective()
{
	Super::ActivateObjective();
	
	if (GetState() == EMissionState::Completed)
	{
		UE_LOG(LogMissions, Error, 
			TEXT("[UMissionInteractiveObjectObjective] BUG - Objective %s woke up already COMPLETED! Force resetting."), 
			*GetName());
		// Force reset state here if possible
	}
	
	AActor* ActorGet = InteractableActor.Get();
	
	if (!ActorGet) // fail if null
	{
		UE_LOG(LogMissions, Warning, 
			TEXT("[UMissionInteractiveObjectObjective]: Error - InteractableActor is null or not loaded!"));
		FailObjective();
		return;
	}
	
	IInteractable* Interactable = Cast<IInteractable>(ActorGet);
		
	if (!Interactable)
	{
		UE_LOG(LogMissions, Warning, 
			TEXT("[UMissionInteractiveObject]: Error - InteractableActor is not IInteractable!"));
		FailObjective();
		return;
	}
	
	IInteractable::Execute_SetInteractable(ActorGet, true);
	
	// just cache this so GetObjectiveLocation() won't cast every time
	CachedActor = ActorGet;
	CachedInteractable.SetObject(ActorGet);
	CachedInteractable.SetInterface(Interactable);
	
	CachedInteractable->GetOnFinishInteract().AddDynamic(this, 
		&UMissionInteractiveObjectObjective::OnInteractFinished);
}

void UMissionInteractiveObjectObjective::DeactivateObjective()
{
	// unsub
	if (CachedInteractable)
	{
		CachedInteractable->GetOnFinishInteract().RemoveDynamic(this,
			&UMissionInteractiveObjectObjective::OnInteractFinished);
	}
	
	Super::DeactivateObjective();
}
// ---- GETTERS ----

AActor* UMissionInteractiveObjectObjective::GetActorTarget() const
{
	if (CachedActor)
		return CachedActor;
	return nullptr; 
}

// ---- INTERACTABLE OBJECT ----

void UMissionInteractiveObjectObjective::OnInteractFinished(TScriptInterface<IInteractable> Interactable, 
	AActor* InteractingActor, bool bSuccess)
{
	IInteractable::Execute_SetInteractable(CachedActor, false);
	
	if (bSuccess)
		CompleteObjective();
}

// --- VALIDATE ----

EDataValidationResult UMissionInteractiveObjectObjective::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
	if (InteractableActor.IsNull())
	{
		Context.AddError(FText::FromString(
			TEXT("[UMissionInteractiveObjectObjective] Error - InteractableActor is not set")));
		return EDataValidationResult::Invalid;
	}
	
	if (AActor* ActorGet = InteractableActor.Get())
	{
		if (!ActorGet->Implements<UInteractable>())
		{
			Context.AddError(FText::FromString(FString::Printf(
			TEXT("[UMissionPointReachObjective] Error - '%s' does not implement IInteractable"), 
				*GetName())));
			Result = EDataValidationResult::Invalid;
		}
	}
	
	return Result;
}
