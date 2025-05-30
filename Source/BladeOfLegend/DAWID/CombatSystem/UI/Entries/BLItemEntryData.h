// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BLCombatUtilities.h"
#include "BLItemEntryData.generated.h"

class UTexture2D;

DECLARE_DELEGATE_OneParam(FOnDeleteFromList, int32 /*Index*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLItemEntryData : public UObject
{
	GENERATED_BODY()

public:
	void Init(int32 InIndex, const FText& InName, ECombatActionFlow InActionFlow, UTexture2D* InThumbnail, bool ContainItem);

public:
	FOnDeleteFromList OnDeleteFromList;
	
	int32 Index;
	FText Name;
	ECombatActionFlow ActionFlow;
	bool bContainItem;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> Thumbnail;
};
