// Test assignment - Fraisins 2026


#include "MissionEntryWidget.h"

#include "Components/TextBlock.h"
#include "UE5G_TestAssignment/Mission/MissionInstance.h"

// ---- BASIC FUNCTIONS ----

void UMissionEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}

// ---- MISSIONS ----

void UMissionEntryWidget::SetMissionInstance(UMissionInstance* MissionInstance)
{
	if (!MissionInstance) // clear entry after update if null ref
	{
		if (bHasMissionEntry)
		{
			ClearEntry();
			return;
		}
		return;
	}
	
	CurrentMission = MissionInstance;
	bHasMissionEntry = true;
	
	// rest in blueprint
	SetMissionInfo(MissionInstance);
}

void UMissionEntryWidget::ClearEntry()
{
	bHasMissionEntry = false;
	OnMissionDisappear();
}

void UMissionEntryWidget::SetMissionInfo(UMissionInstance* MissionInstance)
{
	// same mission check
	if (MissionNameTextBlock->GetText().EqualTo(MissionInstance->GetMissionPublicName()))
	{
		// check if descriptions are same
		if (DescriptionTextBlock->GetText().EqualTo(MissionInstance->GetMissionDescription()))
			return; // do nothing
			
		DescriptionTextBlock->SetText(MissionInstance->GetMissionDescription());
		OnMissionChangedDescription();
		return;
	}
	
	// otherwise whole appearance
	MissionNameTextBlock->SetText(MissionInstance->GetMissionPublicName());
	DescriptionTextBlock->SetText(MissionInstance->GetMissionDescription());
	
	OnMissionAppear();
}
