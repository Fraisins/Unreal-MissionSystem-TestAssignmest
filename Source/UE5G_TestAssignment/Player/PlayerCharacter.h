// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class UInputAction;

UCLASS(Abstract, Blueprintable)
class UE5G_TESTASSIGNMENT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// ---- CONSTRUCTOR ----
	
	APlayerCharacter();
	
	// ---- MOVEMENT ----
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	virtual void SetMovementEnabled(bool bSet) { bMovementEnabled = bSet; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// ---- CAMERA ----
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComponent;
	
	// ---- INPUT ----
	
	UPROPERTY(EditDefaultsOnly, Category="Parameters|Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Parameters|Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Parameters|Input")
	UInputAction* InteractAction;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// ---- ANIMATION ----
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters|Input")
	UAnimMontage* MissionFinishMontage;
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayMissionFinishMontage(FName MissionId);
	
	// ---- MOVEMENT ----
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Parameters|Movement")
	bool bMovementEnabled = true;
	
	virtual void Move(const FInputActionValue& Value);
	virtual bool CanMove();
	
	// ---- LOOK -----
	
	virtual void Look(const FInputActionValue& Value);
	
	// ---- INTERACT ----
	
	UPROPERTY(EditDefaultsOnly, Category="Parameters|Interact")
	float InteractDistance = 200.f;
	
	virtual void Interact();
	
};
