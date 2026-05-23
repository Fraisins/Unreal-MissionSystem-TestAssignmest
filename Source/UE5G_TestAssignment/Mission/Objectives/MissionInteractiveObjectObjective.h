// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "UE5G_TestAssignment/Mission/MissionObjective.h"
#include "MissionInteractiveObjectObjective.generated.h"


class IInteractor;
class IInteractable;

/**
 * Objective that completes when the player interacts with a specific actor.
 * Target actor must implement IInteractable.
 */
UCLASS(BlueprintType)
class UE5G_TESTASSIGNMENT_API UMissionInteractiveObjectObjective : public UMissionObjective
{
	GENERATED_BODY()
	
public:
	// ---- CONSTRUCT ----
	
	UMissionInteractiveObjectObjective();
	
	// ---- BASIC OVERRIDES ----
	
	virtual void ActivateObjective() override;
	virtual void DeactivateObjective() override;
	
	// ---- GETTERS ----
	virtual AActor* GetActorTarget() const override;
	
private:
	
	// ---- INTERACTABLE OBJECT ----
	
	/** Actor the player must interact with. MUST implement IInteractable. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	TSoftObjectPtr<AActor> InteractableActor;
	
	UFUNCTION()
	virtual void OnInteractFinished(TScriptInterface<IInteractable> Interactable, AActor* 
		InteractingActor, bool bSuccess);
	
	// ---- CACHED ----
	UPROPERTY()
	AActor* CachedActor;
    
	TScriptInterface<IInteractable> CachedInteractable;
	
#if WITH_EDITOR
public: // validator
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
};
