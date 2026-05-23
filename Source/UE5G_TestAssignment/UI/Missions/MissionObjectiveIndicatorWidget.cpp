// Test assignment - Fraisins 2026


#include "MissionObjectiveIndicatorWidget.h"

// ---- ANIMATIONS ----

void UMissionObjectiveIndicatorWidget::PlayAppearAnimation()
{
	if (!AppearAnimation)
		return;
	
	PlayAnimation(AppearAnimation);
}

void UMissionObjectiveIndicatorWidget::PlayRemoveAndDestroy()
{
	if (!RemoveAnimation)
	{
		OnRemoveAnimationFinished();
		return;
	}
	FWidgetAnimationDynamicEvent AnimFinished;
	AnimFinished.BindDynamic(this, &UMissionObjectiveIndicatorWidget::OnRemoveAnimationFinished);
	BindToAnimationFinished(RemoveAnimation, AnimFinished);
	
	PlayAnimation(RemoveAnimation);
}

void UMissionObjectiveIndicatorWidget::OnRemoveAnimationFinished()
{
	RemoveFromParent();
	OnPlayRemoveAnimFinished.ExecuteIfBound();
}

