// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

class UMissionListWidget;

/**
 * Main HUD;
 */
UCLASS(Abstract, Blueprintable)
class UE5G_TESTASSIGNMENT_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UMissionListWidget* MissionsList;
};
