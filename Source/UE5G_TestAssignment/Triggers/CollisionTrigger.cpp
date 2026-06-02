// Test assignment - Fraisins 2026


#include "CollisionTrigger.h"
#include "Components/ShapeComponent.h"

// ---- BASIC FUNCTIONS ----

void ACollisionTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	UShapeComponent* Collision = GetCollision();
	if (!Collision)
	{
		UE_LOG(LogCollision, Warning, TEXT("[%s] Error - GetCollision() Not found a valid Collision!"), 
			*GetNameSafe(this));
		return;
	}
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACollisionTrigger::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ACollisionTrigger::OnOverlapEnd);
	
	bEnabledAtStart? TryActivateTrigger() : TryDisableTrigger();
}

void ACollisionTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UShapeComponent* Collision = GetCollision();
	if (!IsValid(Collision))
	{
		UE_LOG(LogCollision, Warning, TEXT("[%s] Error - GetCollision() Not found a valid Collision!"), 
			*GetNameSafe(this));
		return;
	}
	
	Collision->OnComponentBeginOverlap.RemoveDynamic(this, &ACollisionTrigger::OnOverlapBegin);
	Collision->OnComponentEndOverlap.RemoveDynamic(this, &ACollisionTrigger::OnOverlapEnd);
	
	Super::EndPlay(EndPlayReason);
}

// ---- ENABLE / DISABLE ----

void ACollisionTrigger::TryActivateTrigger()
{
	UShapeComponent* Collision = GetCollision();
	if (!Collision) return;
	
	ActivationCount++;
	if (ActivationCount == 1)
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACollisionTrigger::TryDisableTrigger()
{
	UShapeComponent* Collision = GetCollision();
	if (!Collision) return;
	
	ActivationCount = FMath::Max(ActivationCount - 1, 0);
	if (ActivationCount == 0)
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// ---- OVERLAP ----

void ACollisionTrigger::OnOverlapBegin(AActor* OtherActor)
{
	if (!OtherActor)
		return;
	
	if (DoesActorBelongsToClass(OtherActor, TargetActorClass))
	{
		OnActorEnterTrigger.Broadcast(this, OtherActor);
	}
}

void ACollisionTrigger::OnOverlapEnd(AActor* OtherActor)
{
	if (!OtherActor)
		return;
	
	if (DoesActorBelongsToClass(OtherActor, TargetActorClass))
	{
		OnActorExitTrigger.Broadcast(this, OtherActor);
	}
}

bool ACollisionTrigger::DoesActorBelongsToClass(AActor* Actor, TSubclassOf<AActor> TargetClass)
{
	if (!Actor || !TargetClass)
		return false;
	return Actor->IsA(TargetClass);
}

