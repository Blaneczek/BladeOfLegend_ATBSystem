// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/BLCombatUtilities.h"
#include "BLItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MoneyCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type = EItemType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemClass ItemClass = EItemClass::NONE;
};
