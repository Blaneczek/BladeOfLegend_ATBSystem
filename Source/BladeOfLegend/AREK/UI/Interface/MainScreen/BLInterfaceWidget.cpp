// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLInterfaceWidget.h"
#include "PaperFlipbook.h"
#include "BLInterfaceCharacterSlot.h"
#include "BladeOfLegend/AREK/GameInstance/BLGameInstance.h"
#include "BladeOfLegend/AREK/UI/TextBase.h"
#include "Components/VerticalBox.h"
#include "Data/BLHeroDataAsset.h"
#include "Kismet/GameplayStatics.h"

void UBLInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeGameInstance();

	SetCharacterSlots();
	SetTeamInfo();
}

void UBLInterfaceWidget::ToggleMenu()
{
	InitializePlayerController();

	if (!IsInViewport())
	{
		AddToViewport();
		SetUIMode(true);
		return;
	}

	if (!IsVisible())
	{
		if (!SubMenus.IsEmpty())
		{
			for (const TObjectPtr<UUserWidget> SubMenu : SubMenus)
			{
#pragma region NullChecks
				if (!SubMenu)
				{
					UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::ToggleMenu|SubMenu is nullptr"))
					return;
				}
#pragma endregion NullChecks

				SubMenu->SetVisibility(ESlateVisibility::Hidden);
			}
			SubMenus.Empty();
			SetUIMode(false);
		}
		else
		{
			SetVisibility(ESlateVisibility::Visible);
			SetUIMode(true);
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		SetUIMode(false);
	}
}

void UBLInterfaceWidget::AddSubMenu(UUserWidget* SubMenu)
{
#pragma region NullChecks
	if (!SubMenu)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::AddSubMenu|SubMenu is nullptr"))
		return;
	}
#pragma endregion NullChecks

	SubMenus.Add(SubMenu);
}

void UBLInterfaceWidget::RemoveSubMenu(UUserWidget* SubMenu)
{
#pragma region NullChecks
	if (!SubMenu)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::RemoveSubMenu|SubMenu is nullptr"))
		return;
	}
#pragma endregion NullChecks

	SubMenus.Remove(SubMenu);
}

void UBLInterfaceWidget::SetUIMode(const bool bUI) const
{
#pragma region NullChecks
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::SetUIMode|PlayerController is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const FInputModeGameOnly InputModeGameOnly;
	const FInputModeGameAndUI InputModeGameAndUI;
	bUI ? PlayerController->SetInputMode(InputModeGameAndUI) : PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->SetShowMouseCursor(bUI);
	UGameplayStatics::SetGamePaused(this, bUI);
}

void UBLInterfaceWidget::InitializeGameInstance()
{
	BLGameInstance = Cast<UBLGameInstance>(GetGameInstance());
}

void UBLInterfaceWidget::InitializePlayerController()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
}

void UBLInterfaceWidget::SetCharacterSlots() const
{
#pragma region NullChecks
	if (!BLGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::SetCharacterSlots|BLGameInstance is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const int CharacterSlotsNum{VB_CharacterSlots->GetAllChildren().Num()};
	const FHeroDataAssetData HeroDataAssetData{BLGameInstance->GetSaveGameData().HeroesData};
	const TArray<FHeroAssetInfo> HeroesAssetInfo{HeroDataAssetData.Heroes};

	for (int i = 0; i < CharacterSlotsNum; i++)
	{
		UBLInterfaceCharacterSlot* CharacterSlot = Cast<UBLInterfaceCharacterSlot>(
			VB_CharacterSlots->GetChildAt(i));

#pragma region NullChecks
		if (!CharacterSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::SetCharacterSlots|CharacterSlot is nullptr"))
			return;
		}
#pragma endregion NullChecks

		if (HeroesAssetInfo.IsValidIndex(i))
		{
			const FHeroAssetInfo CurrentHeroAssetInfo{HeroesAssetInfo[i]};

			CharacterSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			UPaperSprite* CharacterSprite{CurrentHeroAssetInfo.RealSprite};
			CharacterSlot->SetCharacterImage(CharacterSprite);

			const FString CharacterNameString{CurrentHeroAssetInfo.HeroAttributes.Name};
			CharacterSlot->SetCharacterName(CharacterNameString);

			const ECrystalColor CrystalColor{CurrentHeroAssetInfo.CurrentCrystal};
			CharacterSlot->SetCharacterCrystalColor(CrystalColor);

			const int32 CharacterLevel{HeroDataAssetData.Level};
			const int32 CharacterExperience{HeroDataAssetData.Experience};
			CharacterSlot->SetCharacterLevel(CharacterLevel, CharacterExperience);

			const int32 CharacterExperienceNextLevel{HeroDataAssetData.ExperienceNextLevel};
			CharacterSlot->SetCharacterNextLevelExp(CharacterExperienceNextLevel);

			const float CharacterCurrentHP{CurrentHeroAssetInfo.HeroAttributes.CurrentHP};
			const float CharacterMaxHP = BLGameInstance->GetMaxHP(i);
			CharacterSlot->SetCharacterHP(CharacterCurrentHP, CharacterMaxHP);

			const float CharacterCurrentME{CurrentHeroAssetInfo.HeroAttributes.CurrentME};
			const float CharacterMaxME = BLGameInstance->GetMaxME(i);
			CharacterSlot->SetCharacterME(CharacterCurrentME, CharacterMaxME);
		}
		else
		{
			// If heroes[i] doesnt exist hide characterslot
			CharacterSlot->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBLInterfaceWidget::SetTeamInfo() const
{
#pragma region NullChecks
	if (!BLGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::SetTeamInfo|BLGameInstance is nullptr"))
		return;
	}
	if (!TB_Gold)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceWidget::SetTeamInfo|TB_Gold is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const FHeroDataAssetData HeroDataAssetData{BLGameInstance->GetSaveGameData().HeroesData};

	const int32 Money = HeroDataAssetData.Money;
	const FString MoneyString = FString::Printf(TEXT("Gold: %d"), Money);
	const FText MoneyText = FText::FromString(MoneyString);

	TB_Gold->SetTextText(MoneyText);
}
