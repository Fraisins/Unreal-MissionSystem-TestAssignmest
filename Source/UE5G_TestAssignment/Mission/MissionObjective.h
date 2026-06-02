// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "MissionTypes.h"
#include "UObject/Object.h"
#include "MissionObjective.generated.h"

class UMissionObjective;
class AObjectiveIndicator;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectiveStateChanged, UMissionObjective* Objective,
                                             EMissionState NewState);

/**
 * Single mission objective, responsible for handling it's own progress.
 * Exist as data in UMissionDataAsset, but copied in runtime UMissionInstance.
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class UE5G_TESTASSIGNMENT_API UMissionObjective : public UObject
{
	GENERATED_BODY()
	
public:
	// ---- OBJECTIVE ----
	
	/** Activates objective and starts tracking its progress. */
	virtual void ActivateObjective();
	
	/** Forces objective to complete (for debug, testing, manual control, etc...)
	 * Normally called internally when mission encountered error, or any objective  complete . */
	virtual void ForceComplete();
	
	/** Forces objective to fail (for debug, testing, manual control, etc...)
	 * Normally called internally when mission encountered error, or any objective failed. */
	virtual void ForceFail();
	
	FOnObjectiveStateChanged OnObjectiveStateChanged;
	
	// ---- GETTERS ----
	
	/** Get current objective state */
	UFUNCTION(BlueprintPure, Category="Mission|Getters")
	FORCEINLINE EMissionState GetState() const { return State; }
	
	/** Get mission description 
	 * Can be edited in child classes */
	UFUNCTION(BlueprintPure, Category="Mission|Getters")
	virtual FText GetDescription() const { return Description; }
	
	/** Get target actor if have one (otherwise nullptr)
	 * Used for attaching objective indicator on targets */
	UFUNCTION(BlueprintPure, Category="Mission|Getters")
	virtual AActor* GetActorTarget() const { return nullptr; }
	
	// ---- VALIDATE (for children) ----
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override
		{ return Super::IsDataValid(Context); }
#endif
	
protected:
	
	virtual void FailObjective(); 
	virtual void CompleteObjective();
	
	// ---- INFO ----
	
	/** Mission description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
	FText Description = FText::FromString("Do that... and that...");
	
	// ---- OBJECTIVE ----
	
	/** Used to make objective clear itself from activated state: unsubscribe from tracking level objects, etc...
	 * Should be overriden in child classes */
	virtual void DeactivateObjective() {}

private:
	// ---- STATE ----
	
	UPROPERTY()
	EMissionState State = EMissionState::Inactive;
	
	void ChangeState(EMissionState NewState);
};
