// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "TextBase.h"
#include "Components/TextBlock.h"

void UTextBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetTextBaseParameters(Text, Font, ColorAndOpacityBase, Justification);
}

void UTextBase::SetTextText(const FText NewText) const
{
#pragma region NullChecks
	if (!T_Text)
	{
		UE_LOG(LogTemp, Warning, TEXT("UTextBase::SetText|T_Text is nullptr"))
		return;
	}
#pragma endregion NullChecks

	T_Text->SetText(NewText);
}

void UTextBase::SetFontText(const FSlateFontInfo NewFont) const
{
#pragma region NullChecks
	if (!T_Text)
	{
		UE_LOG(LogTemp, Warning, TEXT("UTextBase::SetFontText|T_Text is nullptr"))
		return;
	}
#pragma endregion NullChecks

	T_Text->SetFont(NewFont);
}

void UTextBase::SetColorAndOpacityText(const FSlateColor NewColorAndOpacity) const
{
#pragma region NullChecks
	if (!T_Text)
	{
		UE_LOG(LogTemp, Warning, TEXT("UTextBase::SetColorAndOpacityText|T_Text is nullptr"))
		return;
	}
#pragma endregion NullChecks

	T_Text->SetColorAndOpacity(NewColorAndOpacity);
}

void UTextBase::SetJustificationText(const ETextJustify::Type NewJustification) const
{
#pragma region NullChecks
	if (!T_Text)
	{
		UE_LOG(LogTemp, Warning, TEXT("UTextBase::SetJustification|T_Text is nullptr"))
		return;
	}
#pragma endregion NullChecks

	T_Text->SetJustification(NewJustification);
}

void UTextBase::SetTextBaseParameters(const FText NewText, const FSlateFontInfo NewFont,
                                      const FSlateColor NewColorAndOpacity,
                                      const ETextJustify::Type NewJustification) const
{
#pragma region NullChecks
	if (!T_Text)
	{
		UE_LOG(LogTemp, Warning, TEXT("UTextBase::SetTextBaseParameters|T_Text is nullptr"))
		return;
	}
#pragma endregion NullChecks

	T_Text->SetText(NewText);
	T_Text->SetFont(NewFont);
	T_Text->SetColorAndOpacity(NewColorAndOpacity);
	T_Text->SetJustification(NewJustification);
}
