// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveIndicator.generated.h"

enum class EMissionState : uint8;
class UMissionObjective;
class UWidgetComponent;
class UMissionObjectiveIndicatorWidget;

/**
 * World-space indicator attached to an objective target actor.
 * Displays a screen-space widget component and destroys itself (with animation) when the objective ends.
 */
UCLASS()
class UE5G_TESTASSIGNMENT_API AObjectiveIndicator : public AActor
{
	GENERATED_BODY()

public:
	AObjectiveIndicator();
	
	// ---- ACTIVATE / REMOVE ----
	
	/** Activates object indicator with objective he should represent */
	void Activate(UMissionObjective* Objective);
	
	// ---- PARENT ----
	
	/** Set an actor indicator will be attached to */
	void SetParent(AActor* OtherActor);

protected:
	virtual void BeginPlay() override;
	
	// ---- WIDGET ANIMATION ----
	
	void PlayAppearAnimation();
	void PlayAnimationAndDestroy();
	
	// ---- WIDGET ----
	
	/** Root point for WidgetComponent */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* WidgetPoint;
	
	/** Indicator widget component (screen) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UWidgetComponent* WidgetComponent;
	
	/** Widget class that widget component uses */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters")
	TSubclassOf<UMissionObjectiveIndicatorWidget> WidgetClass;
	
private:
	// ---- OBJECTIVE ---
	
	UPROPERTY()
	TWeakObjectPtr<UMissionObjective> CachedObjective;
	
	void ClearFromCurrentObjective();
	
	UFUNCTION()
	void OnObjectiveStateChanged(UMissionObjective* Objective, EMissionState NewState);
	
	// ---- PARENT ----
	
	UPROPERTY()
	TWeakObjectPtr<AActor> ParentActor;
	
	// ---- LIFETIME ----
	UFUNCTION()
	void DestroyFinally();
};
