// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLItem.h"
#include "BLCombatItem.generated.h"

class UBLAction;

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLCombatItem : public UBLItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLAction> Action;
};
