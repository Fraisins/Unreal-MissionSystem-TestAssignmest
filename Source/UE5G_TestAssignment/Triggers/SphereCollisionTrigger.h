// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "CollisionTrigger.h"
#include "Components/SphereComponent.h"
#include "SphereCollisionTrigger.generated.h"

class USphereComponent;

/**
 * Collision trigger with sphere collision.
 */
UCLASS()
class UE5G_TESTASSIGNMENT_API ASphereCollisionTrigger : public ACollisionTrigger
{
	GENERATED_BODY()

public:
	// ---- CONSTRUCT ----
	ASphereCollisionTrigger();
	
	virtual UShapeComponent* GetCollision() const override { return SphereCollision; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereCollision;
};
