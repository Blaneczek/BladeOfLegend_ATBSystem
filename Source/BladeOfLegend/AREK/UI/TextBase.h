// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBase.generated.h"

class UTextBlock;

UCLASS()
class BLADEOFLEGEND_API UTextBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetTextText(const FText NewText) const;
	UFUNCTION(BlueprintCallable)
	void SetFontText(FSlateFontInfo NewFont) const;
	UFUNCTION(BlueprintCallable)
	void SetColorAndOpacityText(FSlateColor NewColorAndOpacity) const;
	UFUNCTION(BlueprintCallable)
	void SetJustificationText(ETextJustify::Type NewJustification) const;

	UFUNCTION(BlueprintCallable)
	void SetTextBaseParameters(FText NewText, FSlateFontInfo NewFont, FSlateColor NewColorAndOpacity,
	                           ETextJustify::Type NewJustification) const;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ExposeOnSpawn=true, AllowPrivateAccess=true))
	FText Text;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ExposeOnSpawn=true, AllowPrivateAccess=true))
	FSlateFontInfo Font;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ExposeOnSpawn=true, AllowPrivateAccess=true))
	FSlateColor ColorAndOpacityBase;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ExposeOnSpawn=true, AllowPrivateAccess=true))
	TEnumAsByte<ETextJustify::Type> Justification;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<UTextBlock> T_Text;
};
