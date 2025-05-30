// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLInterfaceCharacterSlot.generated.h"

enum class ECrystalColor : uint8;
class UTextBase;
class UPaperSprite;
class UImage;

UCLASS()
class BLADEOFLEGEND_API UBLInterfaceCharacterSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCharacterImage(UPaperSprite* CharacterImage) const;
	void SetCharacterName(const FString& Name) const;
	void SetCharacterCrystalColor(ECrystalColor CrystalColor) const;
	void SetCharacterLevel(const int32 Level, const int32 Experience) const;
	void SetCharacterNextLevelExp(const int32 ExperienceNextLevel) const;
	void SetCharacterHP(const float CurrentHP, const float MaxHP) const;
	void SetCharacterME(const float CurrentMagicEnergy, const float MaxMagicEnergy) const;

private:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UImage> I_Character;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UTextBase> TB_Name;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UTextBase> TB_CrystalColor;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UTextBase> TB_Level;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UTextBase> TB_NextLevelExp;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UTextBase> TB_Health;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UTextBase> TB_MagicEnergy;
};
