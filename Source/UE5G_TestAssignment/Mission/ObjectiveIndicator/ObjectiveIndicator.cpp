// Test assignment - Fraisins 2026


#include "ObjectiveIndicator.h"

#include "HaveMissionIndicatorPoint.h"
#include "Components/WidgetComponent.h"
#include "UE5G_TestAssignment/Mission/MissionObjective.h"
#include "UE5G_TestAssignment/UI/Missions/MissionObjectiveIndicatorWidget.h"


// ---- CONSTRUCT ----

AObjectiveIndicator::AObjectiveIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WidgetPoint = CreateDefaultSubobject<USceneComponent>(FName("RootPoint"));
	SetRootComponent(WidgetPoint);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("WidgetComponent"));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // look at camera through walls please
	WidgetComponent->SetupAttachment(WidgetPoint);
}

// ---- BASIC FUNCTIONS ----

void AObjectiveIndicator::BeginPlay()
{
	Super::BeginPlay();
	
	if (WidgetClass)
	{
		WidgetComponent->SetWidgetClass(WidgetClass);
		PlayAppearAnimation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Error - WidgetClass is null"), *GetNameSafe(this));
	}
}

// ---- ACTIVATE / REMOVE ----

void AObjectiveIndicator::Activate(UMissionObjective* Objective)
{
	if (!Objective)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Trying to activate objective indicator with invalid objective!"),
			*GetNameSafe(this))
		return;
	}
	
	CachedObjective = Objective;
	CachedObjective->OnObjectiveStateChanged.AddDynamic(this, &AObjectiveIndicator::OnObjectiveStateChanged);
}

// ---- OBJECTIVE ----

void AObjectiveIndicator::ClearFromCurrentObjective()
{
	if (!CachedObjective.IsValid())
		return;
	
	CachedObjective->OnObjectiveStateChanged.RemoveDynamic(this, &AObjectiveIndicator::OnObjectiveStateChanged);
	CachedObjective = nullptr;
}

void AObjectiveIndicator::OnObjectiveStateChanged(UMissionObjective* Objective, EMissionState NewState)
{
	if (NewState == EMissionState::Failed // if failed
		|| NewState == EMissionState::Completed // if completed
		|| NewState == EMissionState::Inactive) // or if canceled
	{
		ClearFromCurrentObjective();
		PlayAnimationAndDestroy();
	}
}

// ---- PARENT ----

void AObjectiveIndicator::SetParent(AActor* OtherActor)
{
	if (!IsValid(OtherActor)) return; // if parent invalid just skip
	
	// set to component (should have interface pointing where to attach)
	if (OtherActor->Implements<UHaveMissionIndicatorPoint>())
	{
		USceneComponent* Point = IHaveMissionIndicatorPoint::Execute_GetMissionIndicatorPoint(OtherActor);
		if (Point)
		{
			AttachToComponent(Point,  FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			ParentActor = OtherActor;
			return;
		}
	}
	// attach to actor otherwise
	AttachToActor(OtherActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ParentActor = OtherActor;
}

void AObjectiveIndicator::PlayAppearAnimation()
{
	UMissionObjectiveIndicatorWidget* IndicatorWidget = 
		Cast<UMissionObjectiveIndicatorWidget>(WidgetComponent->GetUserWidgetObject());
	if (!IndicatorWidget) return;
	
	IndicatorWidget->PlayAppearAnimation();
}

void AObjectiveIndicator::PlayAnimationAndDestroy()
{
	UMissionObjectiveIndicatorWidget* IndicatorWidget = 
		Cast<UMissionObjectiveIndicatorWidget>(WidgetComponent->GetUserWidgetObject());
	if (!IndicatorWidget) return;
	
	IndicatorWidget->OnPlayRemoveAnimFinished.BindUFunction(this, 
		GET_FUNCTION_NAME_CHECKED(AObjectiveIndicator, DestroyFinally));
	
	IndicatorWidget->PlayRemoveAndDestroy();
}

void AObjectiveIndicator::DestroyFinally()
{
	Destroy();
}

