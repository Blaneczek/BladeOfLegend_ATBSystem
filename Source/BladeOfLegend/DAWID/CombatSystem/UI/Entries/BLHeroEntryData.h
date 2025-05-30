// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLHeroEntryData.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroEntryData : public UObject
{
	GENERATED_BODY()

public:
	/** Sets the data needed for UI */
	void Init(int32 InIndex, const FText& InName, float InMaxHP, float InCurrentHP, float InMaxME);

public:
	int32 Index;
	FText Name;
	float MaxHP;
	float MaxME;
	float CurrentHP;
	float CurrentME;
};
