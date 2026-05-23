// Test assignment - Fraisins 2026


#include "MissionListWidget.h"
#include "MissionEntryWidget.h"
#include "Components/VerticalBox.h"
#include "UE5G_TestAssignment/Mission/MissionSubsystem.h"

// ---- BASIC FUNCTIONS ----

void UMissionListWidget::NativeConstruct()
{
	if (!ensureAlwaysMsgf(MissionEntryClass != nullptr,
		TEXT("[UMissionListWidget] Error - MissionEntryClass is NOT assigned in Blueprint!")))
		return;
	
	// get subsystem
	MissionSubsystem = GetGameInstance()->GetSubsystem<UMissionSubsystem>();
	if (!ensure(MissionSubsystem)) return;
	
	// sub to subsystem
	MissionSubsystem->OnMissionFailed.AddDynamic(this, &UMissionListWidget::MissionRemoved);
	MissionSubsystem->OnMissionCompleted.AddDynamic(this, &UMissionListWidget::MissionRemoved);
	MissionSubsystem->OnMissionUpdated.AddDynamic(this, &UMissionListWidget::MissionUpdated);
	
	// get rid of everything in template
	MissionList->ClearChildren();
	
	// add permanent entries
	for (int8 i = 0; i < MaxEntryCount; i++)
	{
		UMissionEntryWidget* Entry = CreateWidget<UMissionEntryWidget>
		(GetOwningPlayer(), MissionEntryClass);
		
		if (!Entry){
			UE_LOG(LogTemp, Error, TEXT("[UMissionsHUDWidget] Error - Failed to create UMissionEntryWidget"));
			return;
		}
		
		MissionEntries.Add(Entry);
		MissionList->AddChild(Entry);
		if (i != MaxEntryCount - 1)
			Entry->SetPadding(FMargin(0.0f, 0.0f, 0.0f, MissionsEntryPaddingBottom));
	}
	
	TArray<FName> MissionKeys;
	MissionSubsystem->GetActiveMissions().GetKeys(MissionKeys);
	
	// give missions if already loaded
	for (int i = 0; i < FMath::Min(MaxEntryCount, MissionKeys.Num()); i++)
	{
		IDArrayOrder.Add(MissionKeys[i]);
	}
	
	UpdateEntries();
}

void UMissionListWidget::NativeDestruct()
{
	// unsub from subsystem
	if (MissionSubsystem)
	{
		MissionSubsystem->OnMissionFailed.RemoveDynamic(this, &UMissionListWidget::MissionRemoved);
		MissionSubsystem->OnMissionCompleted.RemoveDynamic(this, &UMissionListWidget::MissionRemoved);
		MissionSubsystem->OnMissionUpdated.RemoveDynamic(this, &UMissionListWidget::MissionUpdated);
	}
	
	Super::NativeDestruct();
}

// ---- MISSIONS ----

void UMissionListWidget::MissionUpdated(FName MissionID)
{
	// add new id if isn't on the screen
	if (!IDArrayOrder.Contains(MissionID))
	{
		IDArrayOrder.Add(MissionID);
		if (IDArrayOrder.Num() >= MaxEntryCount)
		{
			IDArrayOrder.RemoveAt(0);
		}
	}
	
	UpdateEntries();
}

void UMissionListWidget::MissionRemoved(FName MissionID)
{
	if (!IDArrayOrder.Contains(MissionID))
		return;
	
	int8 Ind = IDArrayOrder.IndexOfByKey(MissionID);
	IDArrayOrder.RemoveAt(Ind);
	
	UpdateEntries();
}

void UMissionListWidget::UpdateEntries()
{
	int8 MaxMissions = MissionSubsystem->GetActiveMissions().Num();
	for (int8 i = 0; i < MaxEntryCount; i++)
	{
		FName MissionID;
		UMissionEntryWidget* Entry = MissionEntries[i];
		if (i < MaxMissions)
		{
			MissionID = IDArrayOrder[i];
			UMissionInstance* Instance = MissionSubsystem->GetMissionInstance(MissionID);
			Entry->SetMissionInstance(Instance); // instance to show mission
		}
		else
		{
			// nullptr to hide mission
			Entry->SetMissionInstance(nullptr);
		}
	}
}
