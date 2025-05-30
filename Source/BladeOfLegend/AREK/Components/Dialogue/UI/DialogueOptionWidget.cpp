// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "DialogueOptionWidget.h"

void UDialogueOptionWidget::SetOptionText(const FText& NewOptionText)
{
	OptionText = NewOptionText;
}

void UDialogueOptionWidget::SetOptionIndex(const int32 NewOptionIndex)
{
	OptionIndex = NewOptionIndex;
}

void UDialogueOptionWidget::SetDialogueIndex(const int32 NewDialogueIndex)
{
	DialogueIndex = NewDialogueIndex;
}

void UDialogueOptionWidget::SetOwningDialogueComponent(const TObjectPtr<UDialogueBaseComponent> DialogueComponent)
{
#pragma region NullChecks
	if (!DialogueComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueOptionWidget::SetOwningDialogueComponent|DialogueComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks
	
	OwningDialogueComponent = DialogueComponent;
}

FText UDialogueOptionWidget::GetOptionText() const
{
	return OptionText;
}

int32 UDialogueOptionWidget::GetOptionIndex() const
{
	return OptionIndex;
}

int32 UDialogueOptionWidget::GetDialogueIndex() const
{
	return DialogueIndex;
}

UDialogueBaseComponent* UDialogueOptionWidget::GetOwningDialogueComponent() const
{
	return OwningDialogueComponent;
}
