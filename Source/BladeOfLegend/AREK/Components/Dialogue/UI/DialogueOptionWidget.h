// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueOptionWidget.generated.h"

class UDialogueBaseComponent;
class UImage;
class UButton;
class UTextBase;

UCLASS()
class BLADEOFLEGEND_API UDialogueOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOptionText(const FText& NewOptionText);
	void SetOptionIndex(const int32 NewOptionIndex);
	void SetDialogueIndex(const int32 NewDialogueIndex);

	void SetOwningDialogueComponent(const TObjectPtr<UDialogueBaseComponent> DialogueComponent);

private:
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	FText GetOptionText() const;
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	int32 GetOptionIndex() const;
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	int32 GetDialogueIndex() const;

	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	UDialogueBaseComponent* GetOwningDialogueComponent() const;

	FText OptionText;
	int32 OptionIndex;
	int32 DialogueIndex;

	TObjectPtr<UDialogueBaseComponent> OwningDialogueComponent;
};
