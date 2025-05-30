// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UDialogueOptionWidget;
class UVerticalBox;
class UTextBase;
class UDialogueBaseComponent;

DECLARE_DELEGATE(FOnAutoProgress);

UCLASS()
class BLADEOFLEGEND_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnAutoProgress OnAutoProgress;
	
	void UpdateDialogueText(const int32 DialogueIndex, const FText& Speaker, const FText& Dialogue,
	                        const TArray<FText>& OptionsText);

	void SetOwningDialogueComponent(const TObjectPtr<UDialogueBaseComponent> DialogueComponent);

private:
	void AutoProgress();

	void FillAnswerOptions(const int32 DialogueIndex, TArray<FText> AnswerOptions) const;
	// Calculate how much time dialogue lasts if no options are available
	float CalculateDialogueDelay(const FText& Dialogue) const;

	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="BL|Dialogue")
	TSubclassOf<UDialogueOptionWidget> DialogueOptionWidgetClass;

	TObjectPtr<UDialogueBaseComponent> OwningDialogueComponent;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBase> TB_Dialogue;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBase> TB_Speaker;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VB_OptionsContainer;
};
