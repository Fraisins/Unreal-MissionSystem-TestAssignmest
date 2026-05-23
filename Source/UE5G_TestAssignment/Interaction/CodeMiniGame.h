// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE5G_TestAssignment/Interaction/Interactable.h"
#include "CodeMiniGame.generated.h"

class UPasswordWidget;

/**
 * Interactable actor with a code-password MiniGame
 */
UCLASS()
class UE5G_TESTASSIGNMENT_API ACodeMiniGame : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ACodeMiniGame();
	
	// ---- INTERACTABLE ----
	
	UPROPERTY(BlueprintAssignable, Category="Interactable|Delegates")
	FOnStartInteract OnStartInteract;
	
	UPROPERTY(BlueprintAssignable, Category="Interactable|Delegates")
	FOnFinishInteract OnFinishInteract;
	
	virtual void SetInteractable_Implementation(bool bCanInteract) override;
	virtual void Interact_Implementation(AActor* Instigator) override;
	
	virtual FOnStartInteract& GetOnStartInteract() override 
	{ return OnStartInteract; }
	
	virtual FOnFinishInteract& GetOnFinishInteract() override
	{ return OnFinishInteract; }

protected:
	// ---- BASIC FUNCTIONS ----
	virtual void BeginPlay() override;
	
	// ---- COMPONENTS ----
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DoorPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
	TSubclassOf<UPasswordWidget> PasswordWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	bool bIsInteractable = false;
	
	// ---- MINI GAME FUNCTIONS ---
	
	void StartWidgetMiniGame(AActor* InteractingActor);
	
	UFUNCTION()
	void MiniGameFailed();
	UFUNCTION()
	void MiniGameComplete();
	
	void RemovePasswordWidget();
	
	/** Plays finish animation. Called after successful code entry. */
	UFUNCTION(BlueprintImplementableEvent, Category = "MiniGame")
	void PlayFinishAnimation();
	
	/** Finalizes interaction and broadcasts OnFinishInteract. MUST be called from blueprint after interaction */
	UFUNCTION(BlueprintCallable, Category = "MiniGame")
	void CompleteMiniGameInteraction();
	
private:
	
	UPROPERTY()
	UPasswordWidget* ActivePasswordWidget;
	
	UPROPERTY()
	AActor* CurrentInteractingActor;
};
