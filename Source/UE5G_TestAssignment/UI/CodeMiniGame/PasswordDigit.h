// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PasswordDigit.generated.h"

class UEditableText;

DECLARE_MULTICAST_DELEGATE(FOnDigitEntered);

/**
 * Widget of one digit with input in password widget.
 */
UCLASS(Abstract)
class UE5G_TESTASSIGNMENT_API UPasswordDigit : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// ---- FOCUS ---
	
	void SetFocused(bool bFocused);
	
	// ---- DIGITS ----
	
	void SetDigit(int32 Digit);
	int8 GetDigit() const;
	void ClearDigit();
	
	FOnDigitEntered OnDigitEntered;
	
protected:
	virtual void NativeConstruct() override;
	
	// for easier click on the widget
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UEditableText* DigitInput;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnFocused();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnFocusEnded();
	
private:
	bool bIsCurrentlyFocused = false;
	
	UFUNCTION()
	void OnTextChanged(const FText& Text);
};
