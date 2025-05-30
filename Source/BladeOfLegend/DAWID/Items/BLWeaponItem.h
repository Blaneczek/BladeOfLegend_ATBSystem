// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLItem.h"
#include "BLWeaponItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLWeaponItem : public UBLItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatStatus Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatElementType Element;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownDecrease;
};
