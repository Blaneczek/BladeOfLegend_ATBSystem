// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BLCombatUtilities.h"
#include "BLActionEntryData.generated.h"

DECLARE_DELEGATE_OneParam(FOnNameChange, UBLActionEntryData* /*ActionEntry*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLActionEntryData : public UObject
{
	GENERATED_BODY()
	
public:
	/** Sets the data needed for UI */
	void Init(int32 InIndex, const FText& InName, ECombatActionFlow InActionFlow, ECrystalColor InCrystalColor = ECrystalColor::NONE, float InMECost = 0.f, int32 InTurnsCost = 0, int32 InTargetsNum = 1);

	/** If action is on turn cooldown, adds the number of remaining turns to the action name. */
	void ChangeName(int32 TurnNum);

	/** Goes back to default action name. */
	void ChangeNameToDefault();

public:
	int32 Index;
	FText Name;
	ECrystalColor CrystalColor;
	float MECost;
	int32 TurnsCost;
	int32 TargetsNum;
	ECombatActionFlow ActionFlow;
	
	bool bCanBeUsed = true;
	FText TempName;

	FOnNameChange OnNameChange;
};
