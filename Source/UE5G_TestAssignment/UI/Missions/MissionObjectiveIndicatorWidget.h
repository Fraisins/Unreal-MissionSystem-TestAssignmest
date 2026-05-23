// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionObjectiveIndicatorWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FAnimFinished);

/**
 * Widget indicator for UMissionHUDWidget
 */
UCLASS(Abstract, Blueprintable)
class UE5G_TESTASSIGNMENT_API UMissionObjectiveIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// ---- ADD / REMOVE ANIMATION ----
	
	void PlayAppearAnimation();
	void PlayRemoveAndDestroy();
	
	FAnimFinished OnPlayRemoveAnimFinished;
	
private:
	
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AppearAnimation;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* RemoveAnimation;
	
	UFUNCTION()
	void OnRemoveAnimationFinished();
	
};
