// Test assignment - Fraisins 2026


#include "SphereCollisionTrigger.h"

// ---- CONSTRUCT ----

ASphereCollisionTrigger::ASphereCollisionTrigger()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(FName("SphereCollision"));
	SphereCollision->SetSphereRadius(100.f);
}

