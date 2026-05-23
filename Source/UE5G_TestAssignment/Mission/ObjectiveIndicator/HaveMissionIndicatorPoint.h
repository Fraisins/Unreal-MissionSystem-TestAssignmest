// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HaveMissionIndicatorPoint.generated.h"

UINTERFACE()
class UHaveMissionIndicatorPoint : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implement this interface on actors that provide specific attachment point (component) for mission indicators.
 * If not implemented, indicator will anchor actor root.
 */
class UE5G_TESTASSIGNMENT_API IHaveMissionIndicatorPoint
{
	GENERATED_BODY()
public:
	// to set in blueprint
	UFUNCTION(BlueprintNativeEvent, Category="Mission")
	USceneComponent* GetMissionIndicatorPoint();
    
	// C++
	virtual USceneComponent* GetMissionIndicatorPoint_Implementation() { return nullptr; }
};
