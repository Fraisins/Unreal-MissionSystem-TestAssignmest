// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PasswordWidget.generated.h"

class UHorizontalBox;
class UPasswordDigit;
class UButton;
class UTextBlock;
class UEditableText;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPassword);

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
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UHorizontalBox* HorizontalBox;
	
	// ---- BINDS ----
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPasswordDigit* Digit1;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPasswordDigit* Digit2;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPasswordDigit* Digit3;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPasswordDigit* Digit4;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* TextBlock;
	
	// ---- Property ----
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	FString CorrectPassword = TEXT("1234");
	
	/** Main label widget text */
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	FText LabelText;
	
	/** Error text. Will replace label if user prints wrong password. */
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	FText ErrorText;

	/** Time of showing error text (then exits the widget with fail minigame result) */
	UPROPERTY(EditDefaultsOnly, Category="Parameters")
	float ErrorDisplayTime = 2.0f;
	
private:
	UPROPERTY()
	TArray<UPasswordDigit*> Digits;
	
	FString GetEnteredPassword() const; // creates result string from all messages
	void SubmitDigits(); // submits the result
	void FocusPasswordDigit(int8 Index); // focus on digit
	
	void ShowError(const FText& Message);
	
	// ---- TIMER ----
	FTimerHandle ErrorTimerHandle;
};
