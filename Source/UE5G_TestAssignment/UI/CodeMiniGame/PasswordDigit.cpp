// Test assignment - Fraisins 2026


#include "PasswordDigit.h"
#include "Components/EditableText.h"

void UPasswordDigit::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (DigitInput)
	{
		if (bIsCurrentlyFocused == false)
			DigitInput->SetIsReadOnly(true);
		DigitInput->OnTextChanged.AddDynamic(this, &UPasswordDigit::OnTextChanged);
	}
}

void UPasswordDigit::SetFocused(bool bFocused)
{
	if (bIsCurrentlyFocused == bFocused)
		return;
	
	bIsCurrentlyFocused = bFocused;
	if (bFocused)
	{
		DigitInput->SetIsReadOnly(false);
		DigitInput->SetKeyboardFocus();
		DigitInput->SetText(FText::GetEmpty()); // ! clear text so it will print new number immediately
		OnFocused();
	}
	else
	{
		DigitInput->SetIsReadOnly(true);
		OnFocusEnded();
	}
}

void UPasswordDigit::SetDigit(int32 Digit)
{
	if (Digit < 0 || Digit > 9) return;
	DigitInput->SetText(FText::FromString(FString::FromInt(Digit)));
}

int8 UPasswordDigit::GetDigit() const
{
	FString CurrentStr = DigitInput->GetText().ToString();
	if (CurrentStr.IsEmpty()) return -1;
	return FCString::Atoi(*CurrentStr); // turns to int
}

void UPasswordDigit::ClearDigit()
{
	DigitInput->SetText(FText::GetEmpty());
}

void UPasswordDigit::OnTextChanged(const FText& Text)
{
	FString Str = Text.ToString();
	if (Str.IsEmpty()) return;

	if (Str.Len() > 1)
	{
		// just clear if someone will try to print moree than one
		DigitInput->SetText(FText::GetEmpty());
		return;
	}

	TCHAR Char = Str[0];
	
	if (!FChar::IsDigit(Char)) // empty if not a digit
	{
		DigitInput->SetText(FText::GetEmpty());
		return;
	}
	
	OnDigitEntered.Broadcast(); 
	// we don't really need to broadcast the digit itself, just the fact that it was printed
}
