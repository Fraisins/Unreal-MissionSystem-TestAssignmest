// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "UE5G_TestAssignment/Mission/MissionObjective.h"
#include "MissionPointReachObjective.generated.h"

class ACollisionTrigger;

/**
 * Objective that completes when entity reaches a trigger zone.
 * Requires a ACollisionTrigger placed in the level.
 */
UCLASS(BlueprintType)
class UE5G_TESTASSIGNMENT_API UMissionPointReachObjective : public UMissionObjective
{
	GENERATED_BODY()
	
public:
	// ---- CONSTRUCT ----
	
	UMissionPointReachObjective();
	
	// ---- BASIC OVERRIDES ----
	
	virtual void ActivateObjective() override;
	virtual void DeactivateObjective() override;
	
	// ---- GETTERS ----
	
	virtual AActor* GetActorTarget() const override;
	
private:
	// ---- TRIGGER ----
	
	/** Trigger zone the player must enter to complete this objective. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters", meta=(AllowPrivateAccess = "true"))
	TSoftObjectPtr<ACollisionTrigger> TargetTrigger;
	
	UFUNCTION()
	void OnColliderTriggered(ACollisionTrigger* Trigger, AActor* TriggeredActor);
	
	// ---- CACHED ----
	UPROPERTY();
	ACollisionTrigger* CachedTargetTrigger;
	
#if WITH_EDITOR
public: // validator
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
