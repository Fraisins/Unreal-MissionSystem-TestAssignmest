// Test assignment - Fraisins 2026


#include "CollisionTrigger.h"
#include "GameplayTagAssetInterface.h"
#include "Components/ShapeComponent.h"

// ---- BASIC FUNCTIONS ----
void ACollisionTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	UShapeComponent* Collision = GetCollision();
	if (!Collision)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Error - GetCollision() Not found a valid Collision!"), 
			*GetNameSafe(this));
		return;
	}
	
	Collision->OnComponentBeginOverlap.AddDynamic(this,&ACollisionTrigger::OnOverlapBegin);
}

void ACollisionTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UShapeComponent* Collision = GetCollision();
	if (!Collision)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Error - GetCollision() Not found a valid Collision!"), 
			*GetNameSafe(this));
		return;
	}
	
	Collision->OnComponentBeginOverlap.RemoveDynamic(this,&ACollisionTrigger::OnOverlapBegin);
	Super::EndPlay(EndPlayReason);
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

