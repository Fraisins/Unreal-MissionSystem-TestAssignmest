// Fill out your copyright notice in the Description page of Project Settings.


#include "PasswordWidget.h"

#include "PasswordDigit.h"
#include "Components/TextBlock.h"

void UPasswordWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TextBlock->SetText(LabelText);
	
	Digits = { Digit1, Digit2, Digit3, Digit4 };
	
	for (int8 i = 0; i < Digits.Num(); i++)
	{
		if (!Digits[i]) continue;
		
		Digits[i]->OnDigitEntered.AddLambda([this, i]
		{
			if (i + 1 < Digits.Num())
				FocusPasswordDigit(i + 1);
			else
				SubmitDigits();
		});
	}
	
	FocusPasswordDigit(0);
	
}

FString UPasswordWidget::GetEnteredPassword() const
{
	FString Result;
	for (UPasswordDigit* Digit : Digits)
	{
		int8 Value = Digit->GetDigit();
		Result.Append(Value >= 0 ? FString::FromInt(Value) : TEXT(""));
	}
	return Result;
}

void UPasswordWidget::SubmitDigits()
{
	const FString Entered = GetEnteredPassword();
	
	if (Entered != CorrectPassword)
	{
		ShowError(ErrorText);
		return;
	}
	OnPasswordCorrect.Broadcast();
}

void UPasswordWidget::FocusPasswordDigit(int8 Index)
{
	for (int8 i = 0; i < Digits.Num(); i++)
	{
		if (Digits[i])
			Digits[i]->SetFocused(i == Index);
	}
}

void UPasswordWidget::ShowError(const FText& Message)
{
	if (ErrorText.IsEmpty()) return;
	
	TextBlock->SetText(ErrorText);
	TextBlock->SetVisibility(ESlateVisibility::Visible);
	
	// wrong input timer before releasing the player
	GetWorld()->GetTimerManager().SetTimer(ErrorTimerHandle, [this]()
	{
		OnPasswordWrong.Broadcast();
	}, ErrorDisplayTime, false);
}
