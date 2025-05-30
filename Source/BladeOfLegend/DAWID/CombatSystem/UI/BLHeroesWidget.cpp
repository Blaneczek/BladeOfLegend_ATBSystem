// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroesWidget.h"
#include "Components/TileView.h"
#include "Entries/BLHeroEntryWidget.h"
#include "Entries/BLHeroEntryData.h"

void UBLHeroesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HeroesTileView->OnItemClicked().AddUObject(this, &UBLHeroesWidget::HeroClicked);
}

void UBLHeroesWidget::AddHero(int32 Index, const FString& HeroName, float MaxHP, float CurrentHP, float MaxME)
{
	if (UBLHeroEntryData* Hero = NewObject<UBLHeroEntryData>())
	{
		Hero->Init(Index, FText::FromString(HeroName), MaxHP, CurrentHP, MaxME);
		HeroesTileView->AddItem(Hero);
	}
}

void UBLHeroesWidget::HeroDied(int32 Index)
{
	if (UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index))))
	{
		HeroWidget->GreyOutHero();
	}
}

void UBLHeroesWidget::HighlightsHero(int32 Index, bool bNewColor)
{
	if (UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index))))
	{
		bNewColor ? HeroWidget->HighlightHero() : HeroWidget->UnlightHero();
	}
}

void UBLHeroesWidget::UnlightsHero()
{
	if (IsValid(ClickedHero))
	{
		ClickedHero->UnlightHero();
		ClickedHero = nullptr;
	}
}

void UBLHeroesWidget::UpdateHeroHP(int32 Index, float MaxHP, float CurrentHP)
{
	if (UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index))))
	{
		HeroWidget->UpdateHP(MaxHP, CurrentHP);
	}
}

void UBLHeroesWidget::UpdateHeroME(int32 Index, float MaxME, float CurrentME)
{
	if (UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index))))
	{
		HeroWidget->UpdateME(MaxME, CurrentME);
	}
}

void UBLHeroesWidget::StartHeroCooldownBar(int32 Index, float Cooldown)
{
	if (UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index))))
	{
		HeroWidget->StartCooldownBar(Cooldown);
	}
}

void UBLHeroesWidget::PauseAllCooldownBars(bool bNewPause)
{
	TArray<UUserWidget*> Heroes = HeroesTileView->GetDisplayedEntryWidgets();
	for (const auto& Hero : Heroes)
	{
		UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(Hero);
		if (HeroWidget && !HeroWidget->IsDead())
		{
			bNewPause ? HeroWidget->PauseCooldownBar() : HeroWidget->UnPauseCooldownBar();
		}
	}
}

void UBLHeroesWidget::HeroClicked(UObject* Item)
{
	UBLHeroEntryWidget* Hero = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(Item));
	if (Hero && Hero->CanDoAction())
	{
		UnlightsHero();
		ClickedHero = Hero;
		ClickedHero->HighlightHero();
		OnHeroClicked.ExecuteIfBound(Hero->GetIndex());
	}
}

void UBLHeroesWidget::ResetHeroCooldownBar(int32 Index)
{
	if (UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index))))
	{
		HeroWidget->ResetCooldownBar();
	}
}


