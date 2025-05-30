// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLEnemyEntryData.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLEnemyEntryData : public UObject
{
	GENERATED_BODY()

public:
	/** Sets the data needed for UI */
	void Init(int32 InIndex, const FText& InName, int32 InLevel);

public:
	int32 Index;
	FText Name;
	int32 Level;

};
