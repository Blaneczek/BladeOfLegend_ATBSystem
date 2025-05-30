// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "DialogueWidget.h"
#include "DialogueOptionWidget.h"
#include "BladeOfLegend/AREK/Components/Dialogue/DialogueBaseComponent.h"
#include "BladeOfLegend/AREK/UI/TextBase.h"
#include "Components/VerticalBox.h"

// ReSharper disable once CppMemberFunctionMayBeConst
void UDialogueWidget::AutoProgress()
{
	OnAutoProgress.Execute();
}

void UDialogueWidget::UpdateDialogueText(const int32 DialogueIndex, const FText& Speaker, const FText& Dialogue,
                                         const TArray<FText>& OptionsText)
{
#pragma region NullChecks
	if (!VB_OptionsContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueWidget::UpdateDialogueText|VB_OptionsContainer is nullptr"))
		return;
	}
	if (!TB_Speaker)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueWidget::UpdateDialogueText|TB_Speaker is nullptr"))
		return;
	}
	if (!TB_Dialogue)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueWidget::UpdateDialogueText|TB_Dialogue is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Clears all answer options
	VB_OptionsContainer->ClearChildren();

	// todo change FText::Format into FText::FromString
	// Define named arguments for formatting
	FFormatNamedArguments Args;
	Args.Add(TEXT("Speaker"), Speaker); // Use the name as the key here

	TB_Speaker->SetTextText(FText::Format(NSLOCTEXT("Namespace", "SpeakerText", "{Speaker}"), Args));
	TB_Dialogue->SetTextText(Dialogue);

	// If there are some answer options, add widget options
	if (!OptionsText.IsEmpty())
	{
		FillAnswerOptions(DialogueIndex, OptionsText);
	}
	// If there are no answer options set timer to auto progress
	else
	{
		const float DialogueDelay = CalculateDialogueDelay(Dialogue);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDialogueWidget::AutoProgress, DialogueDelay, false);
	}
}

void UDialogueWidget::SetOwningDialogueComponent(const TObjectPtr<UDialogueBaseComponent> DialogueComponent)
{
#pragma region NullChecks
	if (!DialogueComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueWidget::SetOwningDialogueComponent|DialogueComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	OwningDialogueComponent = DialogueComponent;
}

void UDialogueWidget::FillAnswerOptions(const int32 DialogueIndex, TArray<FText> AnswerOptions) const
{
#pragma region NullChecks
	if (!DialogueOptionWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueWidget::FillAnswerOptions|DialogueOptionWidgetClass is nullptr"))
		return;
	}
	if (!VB_OptionsContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueWidget::FillAnswerOptions|VB_OptionsContainer is nullptr"))
		return;
	}
	if (!OwningDialogueComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueWidget::FillAnswerOptions|OwningDialogueComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Add widget options to the DialogueWidget
	for (int i = 0; i < AnswerOptions.Num(); i++)
	{
		UDialogueOptionWidget* DialogueOptionWidget = CreateWidget<UDialogueOptionWidget>(
			GetWorld(), DialogueOptionWidgetClass);

		DialogueOptionWidget->SetOptionText(AnswerOptions[i]);
		DialogueOptionWidget->SetOptionIndex(i);
		DialogueOptionWidget->SetDialogueIndex(DialogueIndex);
		DialogueOptionWidget->SetOwningDialogueComponent(OwningDialogueComponent);

		// Fill answer options
		VB_OptionsContainer->AddChildToVerticalBox(DialogueOptionWidget);
	}
}

float UDialogueWidget::CalculateDialogueDelay(const FText& Dialogue) const
{
	const FString DialogueString = Dialogue.ToString();
	TArray<FString> ParsedDialogueStringArray;
	DialogueString.ParseIntoArray(ParsedDialogueStringArray, TEXT(" "), true);

	const float DialogueTime = ParsedDialogueStringArray.Num() * 0.3f + 0.3f;

	return DialogueTime;
}
