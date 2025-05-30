// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BLActionEntryWidget.generated.h"

class UBorder;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLActionEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	/** IUserObjectListEntry interface implementation */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void SetData(int32 InIndex, const FText& InName, float InMECost = 0.f, int32 InTargetsNum = 1);

	void SetIsClicked(bool bNewClicked);

public:
	int32 Index;
	int32 TargetsNum;
	float MECost;
	
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBorder> Border;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor DefaultColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor ClickedColor;
};
