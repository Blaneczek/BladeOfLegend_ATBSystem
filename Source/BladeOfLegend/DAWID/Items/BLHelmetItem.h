// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLItem.h"
#include "BLHelmetItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLHelmetItem : public UBLItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatStatusType StatusImmunity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Agility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Wisdom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownDecrease;
};
