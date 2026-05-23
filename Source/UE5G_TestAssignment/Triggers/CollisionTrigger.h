// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollisionTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTriggerActivated, ACollisionTrigger*, Trigger, 
	AActor*, ActivatedByActor);

UCLASS(Abstract)
class UE5G_TESTASSIGNMENT_API ACollisionTrigger : public AActor
{
	GENERATED_BODY()

public:
	// ---- DELEGATE ----
	
	/** Fires when actor with correct criteria enters trigger */
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnTriggerActivated OnActorEnterTrigger;
	
	/** Fires when actor with correct criteria exits trigger */
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnTriggerActivated OnActorExitTrigger;
	
	// ---- GETTERS ----
	
	/** Get collision of this trigger */
	UFUNCTION(BlueprintPure, Category="CollisionTrigger")
	virtual UShapeComponent* GetCollision() const { return nullptr; }

protected:
	// ---- BASIC FUNCTIONS ----
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// ---- OVERLAPPING ACTOR CLASS ----
	
	/** Class criteria. Only subclasses of this class will trigger. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters|Filter")
	TSubclassOf<AActor> TargetActorClass;
	
private:
	// ---- OVERLAP ----
	void OnOverlapBegin(AActor* OtherActor); // simplified
	void OnOverlapEnd(AActor* OtherActor); // simplified
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult) 
		{ OnOverlapBegin(OtherActor); }
    
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
		{ OnOverlapEnd (OtherActor); }
	
	// ---- OVERLAPPING ACTOR CLASS ----
	
	// checks if entered actor fills the crireria
	bool DoesActorBelongsToClass(AActor* Actor, TSubclassOf<AActor> TargetClass);
};
