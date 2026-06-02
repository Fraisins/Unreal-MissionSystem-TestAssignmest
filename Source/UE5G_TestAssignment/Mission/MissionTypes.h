// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "MissionTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMissions, Log, All);

/**
 * Mission and objective state
 */
UENUM(BlueprintType)
enum class EMissionState : uint8
{
	Inactive,
	Active,
	Completed,
	Failed
};