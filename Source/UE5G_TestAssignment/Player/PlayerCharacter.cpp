// Test assignment - Fraisins 2026


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "UE5G_TestAssignment/Interaction/Interactable.h"
#include "UE5G_TestAssignment/Mission/MissionSubsystem.h"

// ---- CONSTRUCT ----

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// set capsule scale
	GetCapsuleComponent()->InitCapsuleSize(50.f, 96.0f);
	
	// set camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(GetMesh(), FName("Head"));
	CameraComponent->bUsePawnControlRotation = true; // ignore bone attachment for rotation
	CameraComponent->bEnableFirstPersonFieldOfView = true;
	CameraComponent->bEnableFirstPersonScale = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// mission subsystem
	UMissionSubsystem* MissionSubsystem = GetGameInstance()->GetSubsystem<UMissionSubsystem>();
	if (!ensure(MissionSubsystem)) return;
	
	MissionSubsystem->OnMissionCompleted.AddDynamic(this, &APlayerCharacter::PlayMissionFinishMontage);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	// mission subsystem
	UMissionSubsystem* MissionSubsystem = GetGameInstance()->GetSubsystem<UMissionSubsystem>();
	if (!ensure(MissionSubsystem)) return;
	
	MissionSubsystem->OnMissionCompleted.RemoveDynamic(this, &APlayerCharacter::PlayMissionFinishMontage);
}

// ---- INPUT ----

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// bind actions
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, 
			&APlayerCharacter::Interact);
	}
	else
		UE_LOG(LogInput, Error, TEXT("[PlayerCharacter] Bind actions failed: UEnhancedInputComponent not found on %s"), 
			*GetNameSafe(this));
}

// ---- MOVEMENT ----

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	float Right = v.X, Forward =  v.Y;
	
	if (!CanMove() || !GetController())
		return;
	
	AddMovementInput(GetActorRightVector(), Right);
	AddMovementInput(GetActorForwardVector(), Forward);
}

bool APlayerCharacter::CanMove()
{
	return bMovementEnabled;
}

// ---- LOOK ----

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	float Yaw = v.X, Pitch =  v.Y;
	
	if (!GetController())
		return;
	
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

// ---- INTERACT ----

void APlayerCharacter::Interact()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	// Берём точку и направление из камеры
	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	const FVector Start = CameraLocation;
	const FVector End = Start + CameraRotation.Vector() * InteractDistance;
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // ignore self!
	
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) return;
		
		//DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 8, FColor::Green, false, 2.0f);
		// UE_LOG(LogTemp, Log, TEXT("Interact: Hit: %s"), *Hit.GetActor()->GetName());
		
		if (HitActor->Implements<UInteractable>())
		{
			IInteractable::Execute_Interact(Hit.GetActor(), this);
		}
	}
}