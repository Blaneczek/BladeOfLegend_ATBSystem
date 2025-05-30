// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSaveGame.h"

void UBLSaveGame::SetSaveData(const FSaveGameData& InSaveData)
{
	SaveData = InSaveData;
}

void UBLSaveGame::GetSaveData(FSaveGameData& OutSaveData)
{
	OutSaveData = SaveData;
}
