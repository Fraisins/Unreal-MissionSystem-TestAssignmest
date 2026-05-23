// Test assignment - Fraisins 2026


#include "CodeMiniGame.h"
#include "Blueprint/UserWidget.h"
#include "UE5G_TestAssignment/Player/MainPlayerController.h"
#include "UE5G_TestAssignment/UI/CodeMiniGame/PasswordWidget.h"


// Sets default values
ACodeMiniGame::ACodeMiniGame()
{
	PrimaryActorTick.bCanEverTick = true; // just for timeline door animation
	
	DoorPoint = CreateDefaultSubobject<USceneComponent>("DoorPoint");
	SetRootComponent(DoorPoint);
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	DoorMesh->SetupAttachment(DoorPoint);
}

// ---- INTERACT ----

void ACodeMiniGame::SetInteractable_Implementation(bool bCanInteract)
{
	IInteractable::SetInteractable_Implementation(bCanInteract);
	bIsInteractable = bCanInteract;
}

void ACodeMiniGame::Interact_Implementation(AActor* InteractingActor)
{
	IInteractable::Interact_Implementation(InteractingActor);
	if (!bIsInteractable)
		return;
	
	StartWidgetMiniGame(InteractingActor);
}

void ACodeMiniGame::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ensureAlwaysMsgf(PasswordWidgetClass != nullptr, 
		TEXT("[%s] Error - MiniGameWidgetClass is NOT assigned in Blueprint!"), *GetNameSafe(this)))
		return;
}

// ---- MINI GAME FUNCTIONS ---

void ACodeMiniGame::StartWidgetMiniGame(AActor* InteractingActor)
{
	if (!InteractingActor) return;
	
	// get controller
	AMainPlayerController* PC = AMainPlayerController::GetMainPlayerControllerFromActor(InteractingActor);
	if (!PC) return;
	
	ActivePasswordWidget = CreateWidget<UPasswordWidget>(PC, PasswordWidgetClass);
	if (!PasswordWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACodeMiniGame] Error - %s failed to create MiniGameWidget"),
			*GetNameSafe(this));
		return;
	}
	
	ActivePasswordWidget->OnPasswordWrong.AddDynamic(this, &ACodeMiniGame::MiniGameFailed);
	ActivePasswordWidget->OnPasswordCorrect.AddDynamic(this, &ACodeMiniGame::MiniGameComplete);
	PC->AddMiniGameToScreen(ActivePasswordWidget);
	
	CurrentInteractingActor = InteractingActor;
	OnStartInteract.Broadcast(TScriptInterface<IInteractable>(this), CurrentInteractingActor);
}

void ACodeMiniGame::MiniGameFailed()
{
	RemovePasswordWidget();
	CurrentInteractingActor = nullptr;
}

void ACodeMiniGame::MiniGameComplete()
{
	RemovePasswordWidget();
	PlayFinishAnimation();
}

void ACodeMiniGame::RemovePasswordWidget()
{
	AMainPlayerController* PC = AMainPlayerController::GetMainPlayerControllerFromActor(CurrentInteractingActor);
	if (!PC) return;
	PC->RemoveCurrentMiniGameFromScreen();
	
	ActivePasswordWidget->OnPasswordWrong.RemoveDynamic(this, &ACodeMiniGame::MiniGameFailed);
	ActivePasswordWidget->OnPasswordCorrect.RemoveDynamic(this, &ACodeMiniGame::MiniGameComplete);
	
	ActivePasswordWidget = nullptr;
}

void ACodeMiniGame::CompleteMiniGameInteraction()
{
	OnFinishInteract.Broadcast(this, CurrentInteractingActor, true);
	CurrentInteractingActor = nullptr;
}

