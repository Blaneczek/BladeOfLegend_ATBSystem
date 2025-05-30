// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLInterfaceCharacterSlot.h"
#include "PaperSprite.h"
#include "BladeOfLegend/AREK/UI/TextBase.h"
#include "Components/Image.h"

void UBLInterfaceCharacterSlot::SetCharacterImage(UPaperSprite* CharacterImage) const
{
#pragma region NullChecks
	if (!I_Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterImage|I_Character is nullptr"))
		return;
	}
	if (!CharacterImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterImage|CharacterImage is nullptr"))
		return;
	}
#pragma endregion NullChecks

	I_Character->SetBrushResourceObject(CharacterImage);
}

void UBLInterfaceCharacterSlot::SetCharacterName(const FString& Name) const
{
#pragma region NullChecks
	if (!TB_Name)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterName|TB_Name is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const FText NameText{FText::FromString(Name)};

	TB_Name->SetTextText(NameText);
}

void UBLInterfaceCharacterSlot::SetCharacterLevel(const int32 Level, const int32 Experience) const
{
#pragma region NullChecks
	if (!TB_Level)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterLevel|TB_Level is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const FString LevelString{FString::Printf(TEXT("Level: %d (%d exp)"), Level, Experience)};
	const FText LevelText{FText::FromString(LevelString)};

	TB_Level->SetTextText(LevelText);
}

void UBLInterfaceCharacterSlot::SetCharacterNextLevelExp(const int32 ExperienceNextLevel) const
{
#pragma region NullChecks
	if (!TB_NextLevelExp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterNextLevelExp|TB_NextLevelExp is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const FString NextLevelExpString{FString::Printf(TEXT("Next lvl exp: %d"), ExperienceNextLevel)};
	const FText NextLevelExpText{FText::FromString(NextLevelExpString)};

	TB_NextLevelExp->SetTextText(NextLevelExpText);
}

void UBLInterfaceCharacterSlot::SetCharacterCrystalColor(ECrystalColor CrystalColor) const
{
#pragma region NullChecks
	if (!TB_CrystalColor)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterCrystal|TB_CrystalColor is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const FString FullEnumString{UEnum::GetValueAsString(CrystalColor)};
	const FString CrystalString{FullEnumString.RightChop((FullEnumString.Find(TEXT("::")) + 2))};
	const FString CrystalColorString{FString::Printf(TEXT("Crystal: %s"), *CrystalString)};
	const FText CrystalColorText{FText::FromString(CrystalColorString)};

	TB_CrystalColor->SetTextText(CrystalColorText);
}

void UBLInterfaceCharacterSlot::SetCharacterHP(const float CurrentHP, const float MaxHP) const
{
#pragma region NullChecks
	if (!TB_Health)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterHealth|TB_Health is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const int CurrentHealthInt{static_cast<int>(CurrentHP)};
	const int MaxHPInt{static_cast<int>(MaxHP)};
	const FString HPString{FString::Printf(TEXT("HP: %d/%d"), CurrentHealthInt, MaxHPInt)};
	const FText HPText{FText::FromString(HPString)};

	TB_Health->SetTextText(HPText);
}

void UBLInterfaceCharacterSlot::SetCharacterME(const float CurrentMagicEnergy,
                                               const float MaxMagicEnergy) const
{
#pragma region NullChecks
	if (!TB_MagicEnergy)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceCharacterSlot::SetCharacterMagicEnergy|TB_MagicEnergy is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const int CurrentMEInt{static_cast<int>(CurrentMagicEnergy)};
	const int MaxMEInt{static_cast<int>(MaxMagicEnergy)};
	const FString MEString{FString::Printf(TEXT("ME: %d/%d"), CurrentMEInt, MaxMEInt)};
	const FText METext{FText::FromString(MEString)};

	TB_MagicEnergy->SetTextText(METext);
}
