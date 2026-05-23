// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// DYNAMIC DELEGATES DECLARATION
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartInteract, TScriptInterface<IInteractable>, InteractableActor, 
	AActor*, InteractingActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFinishInteract, TScriptInterface<IInteractable>, InteractableActor,  
	AActor*, InteractingActor, bool, bSuccess);

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for interactable actors on game
 */
class UE5G_TESTASSIGNMENT_API IInteractable
{
	GENERATED_BODY()

public:
	// ---- INTERACTION ----
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void SetInteractable(bool bCanInteract);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void Interact(AActor* InteractingActor);
	
	// ---- GETTERS (DELEGATE) ----
	virtual FOnStartInteract& GetOnStartInteract() = 0;
	virtual FOnFinishInteract& GetOnFinishInteract() = 0;
};
