// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/BLCombatUtilities.h"
#include "BLSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLSaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	void SetSaveData(const FSaveGameData& InSaveData);
	void GetSaveData(FSaveGameData& OutSaveData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSaveGameData SaveData;

};
