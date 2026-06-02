// Test assignment - Fraisins 2026


#include "PasswordWidget.h"
#include "PasswordDigit.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

#if WITH_EDITOR
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet2/BlueprintEditorUtils.h"
#endif


void UPasswordWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (HorizontalBox && HorizontalBox->GetChildrenCount() > 0)
		InitializeDigits();
}

void UPasswordWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TextBlock->SetText(LabelText);
	FocusPasswordDigit(0);
}

// ---- GENERATION ----

void UPasswordWidget::GenerateDigitWidgets()
{
#if WITH_EDITOR
	if (!DigitClass || !HorizontalBox || !WidgetTree) return;
	
	UWidgetBlueprintGeneratedClass* WidgetBlueprintGeneratedClass = Cast<UWidgetBlueprintGeneratedClass>(GetClass());
	
	UPackage* Package = WidgetBlueprintGeneratedClass->GetPackage();
	UWidgetBlueprint* MainAsset = Cast<UWidgetBlueprint>(Package->FindAssetInPackage());
	
	UHorizontalBox* AssetHorizontalBox = Cast<UHorizontalBox>(
		MainAsset->WidgetTree->FindWidget("HorizontalBox"));
	if (!AssetHorizontalBox) return;
	
	// clear previous digits
	TArray<UWidget*> Children = AssetHorizontalBox->GetAllChildren();
	for (UWidget* Child : Children)
	{
		Child->Modify();
		MainAsset->WidgetTree->RemoveWidget(Child);
	}
	AssetHorizontalBox->ClearChildren();

    // generate new digits
    for (uint8 i = 0; i < MaxDigitsAmount; i++)
    {
        UPasswordDigit* Digit = MainAsset->WidgetTree->ConstructWidget<UPasswordDigit>(
        	DigitClass, 
        	FName(*FString::Printf(TEXT("PasswordDigit_%d"), i)));
        if (!Digit) continue;
		
        UPanelSlot* PanelSlot = AssetHorizontalBox->AddChild(Digit);
        UHorizontalBoxSlot* HorizontalSlot = Cast<UHorizontalBoxSlot>(PanelSlot);
        if (!HorizontalSlot) continue;

    	HorizontalSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
        HorizontalSlot->SetHorizontalAlignment(HAlign_Fill);
        HorizontalSlot->SetVerticalAlignment(VAlign_Fill);

    	if (i < MaxDigitsAmount - 1)
    		HorizontalSlot->SetPadding(FMargin(0.f, 0.f, HorizontalPadding, 0.f));
    	else
    		HorizontalSlot->SetPadding(FMargin(0.f));
    }
	
	AssetHorizontalBox->Modify();
	MainAsset->Modify();
	
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(MainAsset);
#endif
}

void UPasswordWidget::InitializeDigits()
{
	// add digits from box to array for easier use
	Digits.Empty();
	for (uint8 i = 0; i < HorizontalBox->GetChildrenCount(); i++)
	{
		if (UPasswordDigit* Digit = Cast<UPasswordDigit>(HorizontalBox->GetChildAt(i)))
			Digits.Add(Digit);
	}
	
	// bind to digit's delegate
	for (uint8 i = 0; i < Digits.Num(); i++)
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
