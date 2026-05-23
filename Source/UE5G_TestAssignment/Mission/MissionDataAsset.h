// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "MissionObjective.h"
#include "Engine/DataAsset.h"
#include "MissionDataAsset.generated.h"


UCLASS()
class UE5G_TESTASSIGNMENT_API UMissionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mission|ID")
	FName MissionID;
	
	/** FText public name (for UI) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mission|UI")
	FText MissionPublicName;
	
	// ---- OBJECTIVES ----
	
	/** Mission Objectives (activates one after another) */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category="Mission|Objectives")
	TArray<TObjectPtr<UMissionObjective>> MissionObjectives;
	
	/** Indicator class to point where current objective is */
	UPROPERTY(EditDefaultsOnly, Category="Mission|Objectives")
	TSubclassOf<AObjectiveIndicator> ObjectiveIndicatorClass;
	
#if WITH_EDITOR
	// Validator because null objective is pretty possible
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
