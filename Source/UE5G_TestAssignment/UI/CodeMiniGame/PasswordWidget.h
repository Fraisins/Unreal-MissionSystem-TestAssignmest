// Test assignment - Fraisins 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PasswordWidget.generated.h"

class UHorizontalBox;
class UPasswordDigit;
class UButton;
class UTextBlock;
class UEditableText;

DECLARE_MULTICAST_DELEGATE(FOnPassword);

/**
 * Password widget mini game.
 */
UCLASS(Abstract)
class UE5G_TESTASSIGNMENT_API UPasswordWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// ---- DELEGATES ----
	
	FOnPassword OnPasswordCorrect;
	FOnPassword OnPasswordWrong;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// ---- GENERATION ----
	
	UFUNCTION(CallInEditor, Category = "PasswordWidget|Generation")
	void GenerateDigitWidgets();
	
	// ---- BINDS ----
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UHorizontalBox* HorizontalBox;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* TextBlock;
	
	// ---- PROPERTIES ----
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters")
	FString CorrectPassword = TEXT("1234");
	
	/** Main label widget text */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters")
	FText LabelText;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters|Digits")
	uint8 MaxDigitsAmount = 4;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters|Digits")
	TSubclassOf<UPasswordDigit> DigitClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters|Digits")
	float HorizontalPadding = 25;
#endif
	
	/** Error text. Will replace label if user prints wrong password. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters|Error")
	FText ErrorText;

	/** Time of showing error text (then exits the widget with fail minigame result) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters|Error")
	float ErrorDisplayTime = 2.0f;
	
private:
	UPROPERTY()
	TArray<UPasswordDigit*> Digits;
	
	void InitializeDigits();
	FString GetEnteredPassword() const; // creates result string from all messages
	void SubmitDigits(); // submits the result
	void FocusPasswordDigit(int8 Index); // focus on digit
	
	void ShowError(const FText& Message);
	
	// ---- TIMER ----
	FTimerHandle ErrorTimerHandle;
};
