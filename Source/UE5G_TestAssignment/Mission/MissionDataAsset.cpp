// Test assignment - Fraisins 2026


#include "MissionDataAsset.h"
#include "Misc/DataValidation.h"

// ---- VALIDATE ----

EDataValidationResult UMissionDataAsset::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
	// ---- PUBLIC NAME CHECK ----
	if (MissionPublicName.IsEmpty())
	{
		Context.AddError(FText::FromString(FString::Printf(
			TEXT("[UMissionDataAsset] Error: Mission '%s' doesn't have a public naFe!"), 
			*GetName())));
		Result = EDataValidationResult::Invalid;
	}
	
	// ---- OBJECTIVES CHECK ----
	// should have objectives
	if (MissionObjectives.Num() == 0)
	{
		Context.AddError(FText::FromString(FString::Printf(
			TEXT("[UMissionDataAsset] Error: Mission '%s' has 0 objectives!"), 
			*GetName())));
		Result = EDataValidationResult::Invalid;
	}
	// check objectives in for loop
	for (int8 i = 0; i < MissionObjectives.Num(); ++i)
	{
		const UMissionObjective* Objective = MissionObjectives[i];
		
		if (!Objective) // check that objective isn't null
		{
			Context.AddError(FText::FromString(FString::Printf(
			TEXT("[UMissionDataAsset] Error: Mission '%s' has null objective at index [%d]!"), 
			*GetName(), i)));
			Result = EDataValidationResult::Invalid;
		}
		Result = Objective->IsDataValid(Context);
	}
	
	return Result;
}
