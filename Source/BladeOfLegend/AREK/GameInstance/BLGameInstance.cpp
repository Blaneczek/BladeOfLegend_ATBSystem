// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Data/BLHeroDataAsset.h"
#include "BladeOfLegend/BLSaveGame.h"
#include "BladeOfLegend/DAWID/Items/BLArmorItem.h"
#include "BladeOfLegend/DAWID/Items/BLWeaponItem.h"
#include "BladeOfLegend/DAWID/Items/BLHelmetItem.h"
#include "Actions/BLAction.h"
 
void UBLGameInstance::SetCombatData(const FCombatData& InCombatData, const FPostCombatData& InPostCombatData)
{
	CombatData = InCombatData;
	PostCombatData = InPostCombatData;

	if (InPostCombatData.bUseNewPlayerPosition)
	{
		PostCombatData.PlayerPosition = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	}	
}

void UBLGameInstance::SetEnemyFlag(const FName& EnemyTag, bool bDead)
{
	if (SaveGameData.Enemies.Contains(EnemyTag))
	{
		SaveGameData.Enemies[EnemyTag] = bDead;
	}
}

void UBLGameInstance::SaveHeroData()
{
	UBLHeroDataAsset* Data = HeroDataAsset.LoadSynchronous();
	if (Data)
	{
		SaveGameData.HeroesData = Data->GetDataAssetAsStruct();
	}
}

void UBLGameInstance::LoadHeroData()
{
	UBLHeroDataAsset* Data = HeroDataAsset.LoadSynchronous();
	if (Data)
	{
		Data->SetDataAssetFromStruct(SaveGameData.HeroesData);
	}	
}


FCombatCharData UBLGameInstance::CalculateBaseCombatData(int32 Index)
{

	const FString Name = SaveGameData.HeroesData.Heroes[Index].HeroAttributes.Name;

	TSubclassOf<ABLCombatCharacter> Class = SaveGameData.HeroesData.Heroes[Index].Class;

	const float Strength =
		SaveGameData.HeroesData.Heroes[Index].HeroAttributes.Strength +
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->Strength +
		SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->Strength;

	const float Agility =
		SaveGameData.HeroesData.Heroes[Index].HeroAttributes.Agility +
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->Agility +
		SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->Agility;

	const float Wisdom =
		SaveGameData.HeroesData.Heroes[Index].HeroAttributes.Wisdom +
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->Wisdom +
		SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->Wisdom;

	const float Endurance =
		SaveGameData.HeroesData.Heroes[Index].HeroAttributes.Endurance +
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->Endurance +
		SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->Endurance;

	const float MaxHP = GetMaxHP(Index);

	const float MaxME = GetMaxME(Index);

	float BaseAttackDMG =
		SaveGameData.HeroesData.Level +
		SaveGameData.HeroesData.Heroes[Index].HeroAttributes.TotalCrystalsLevel +
		(Strength / 3) +
		SaveGameData.HeroesData.Heroes[Index].Weapon.GetDefaultObject()->Damage;

	BaseAttackDMG = FMath::RoundHalfFromZero(BaseAttackDMG);

	float BaseDefense =
		(SaveGameData.HeroesData.Level / 2) +
		(Agility / 3) +
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->Defense +
		SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->Defense;

	BaseDefense = FMath::RoundHalfFromZero(BaseDefense);

	const float BaseDodge = FMath::RoundHalfFromZero(Agility / 4);

	const float CooldownTemp =
		SaveGameData.HeroesData.Heroes[Index].Cooldown -
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->CooldownDecrease -
		SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->CooldownDecrease -
		SaveGameData.HeroesData.Heroes[Index].Weapon.GetDefaultObject()->
		CooldownDecrease;

	const float Cooldown = FMath::Clamp(CooldownTemp, 1.f, SaveGameData.HeroesData.Heroes[Index].Cooldown);

	const float Pierce = FMath::RoundHalfFromZero(Agility / 5);

	const ECombatElementType Element = SaveGameData.HeroesData.Heroes[Index].Element;

	const ECombatElementType WeaponElement = SaveGameData.HeroesData.Heroes[Index].Weapon.GetDefaultObject()->Element;

	TSet<ECombatStatusType> StatusesImmunity;
	StatusesImmunity.Add(SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->StatusImmunity);
	StatusesImmunity.Add(SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->StatusImmunity);

	const FCombatStatus WeaponStatus = SaveGameData.HeroesData.Heroes[Index].Weapon.GetDefaultObject()->Status;

	UPaperFlipbook* Sprite = SaveGameData.HeroesData.Heroes[Index].Sprite;

	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass = SaveGameData.HeroesData.Heroes[Index].AnimInstanceClass;

	UPaperZDAnimSequence* TakeDMGAnim = SaveGameData.HeroesData.Heroes[Index].TakeDMGAnim;

	const FText SpecialActionsName = SaveGameData.HeroesData.Heroes[Index].SpecialActionsName;

	return FCombatCharData(Name, Class, MaxHP, MaxME, SaveGameData.HeroesData.Heroes[Index].HeroAttributes.CurrentHP,
		SaveGameData.HeroesData.Heroes[Index].HeroAttributes.CurrentME, BaseAttackDMG, BaseDefense
		, BaseDodge, Cooldown, Strength, Agility, Wisdom
		, Endurance, Pierce, Element, WeaponElement, StatusesImmunity
		, WeaponStatus, Sprite, AnimInstanceClass
		, TakeDMGAnim, SpecialActionsName);
}

void UBLGameInstance::AddCombatReward(int32 InExperience, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems)
{
	SaveGameData.HeroesData.Money += InMoney;
	SaveGameData.HeroesData.Items.Append(InItems);

	bLevelUP = false;
	LevelUPData.Empty();

	SaveGameData.HeroesData.Experience += InExperience;
	if (SaveGameData.HeroesData.Experience >= SaveGameData.HeroesData.ExperienceNextLevel)
	{
		LevelUP();
	}
}

float UBLGameInstance::GetMaxHP(int32 Index) const
{
	if (!SaveGameData.HeroesData.Heroes.IsValidIndex(Index))
	{
		return 0.f;
	}

	return SaveGameData.HeroesData.Heroes[Index].HeroAttributes.BaseHealth +
		(5 * SaveGameData.HeroesData.Heroes[Index].HeroAttributes.Endurance) +
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->AdditionalHP +
		(5 * SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->Endurance) +
		(5 * SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->Endurance);
}

float UBLGameInstance::GetMaxME(int32 Index) const
{
	if (!SaveGameData.HeroesData.Heroes.IsValidIndex(Index))
	{
		return 0.f;
	}

	return SaveGameData.HeroesData.Heroes[Index].HeroAttributes.BaseMagicEnergy +
		(5 * SaveGameData.HeroesData.Heroes[Index].HeroAttributes.Wisdom) +
		SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->AdditionalME +
		(5 * SaveGameData.HeroesData.Heroes[Index].Armor.GetDefaultObject()->Wisdom) +
		(5 * SaveGameData.HeroesData.Heroes[Index].Helmet.GetDefaultObject()->Wisdom);
}

void UBLGameInstance::LevelUP()
{
	bLevelUP = true;
	SaveGameData.HeroesData.Level++;
	if (SaveGameData.HeroesData.Level == 10 && SaveGameData.HeroesData.Heroes.IsValidIndex(0) && SaveGameData.HeroesData.Heroes.IsValidIndex(1) && SaveGameData.HeroesData.Heroes[0].CombatActions.SpecialActions.Num() == 1)
	{
		SaveGameData.HeroesData.Heroes[0].CombatActions.SpecialActions.Add(SpecialSkillKrolth);
		SaveGameData.HeroesData.Heroes[1].CombatActions.SpecialActions.Add(SpecialSkillCardie);
	}

	SaveGameData.HeroesData.Experience = FMath::Clamp(SaveGameData.HeroesData.Experience - SaveGameData.HeroesData.ExperienceNextLevel, 0, SaveGameData.HeroesData.ExperienceNextLevel);
	SaveGameData.HeroesData.ExperienceNextLevel += 30;

	const float Strength = 1.f;
	const float Agility = 1.f;
	const float Wisdom = 1.f;
	const float Endurance = 1.f;

	int32 TempIndex = 0;

	for (auto& Hero : SaveGameData.HeroesData.Heroes)
	{
		float CrystalStrength = 0.f;
		float CrystalAgility = 0.f;
		float CrystalWisdom = 0.f;
		float CrystalEndurance = 0.f;
		ECrystalColor CrystalColor = ECrystalColor::NONE;
		bool bNewSkill = false;
		FText SkillName;
		UPaperSprite* CrystalSprite = nullptr;

		int32 CrystalSkillIndex = 0;
		CrystalColor = Hero.CurrentCrystal;

		if (Hero.CrystalsData.Contains(CrystalColor))
		{
			const int32 CrystalLevel = ++Hero.CrystalsData.Find(CrystalColor)->Level;
			CrystalSprite = Hero.CrystalsData.Find(CrystalColor)->CrystalSprite;

			if (CrystalLevel % 5 == 0)
			{
				bNewSkill = true;
				CrystalSkillIndex = CrystalLevel / 5 - 1;
			}

			if (bNewSkill && Hero.CrystalsData.Find(CrystalColor)->Skills.IsValidIndex(CrystalSkillIndex))
			{
				if (!Hero.CombatActions.CrystalActions.Contains(CrystalColor))
				{
					Hero.CombatActions.CrystalActions.Add(CrystalColor, FCrystalSkills());
				}

				Hero.CombatActions.CrystalActions.Find(CrystalColor)->Skills.Add(Hero.CrystalsData.Find(CrystalColor)->Skills[CrystalSkillIndex]);

				if (UBLAction* Action = Cast<UBLAction>(Hero.CrystalsData.Find(CrystalColor)->Skills[CrystalSkillIndex].LoadSynchronous()->GetDefaultObject()))
				{
					SkillName = Action->Name;
				}
			}
			switch (CrystalColor)
			{
			case ECrystalColor::RED:
			{
				if (CrystalLevel % 3 == 0)
				{
					CrystalAgility = 1.f;
					CrystalWisdom = 1.f;
				}
				CrystalStrength = 3.f;
				CrystalEndurance = 3.f;
				break;
			}
			case ECrystalColor::PURPLE:
			{
				if (CrystalLevel % 3 == 0)
				{
					CrystalAgility = 1.f;
					CrystalEndurance = 1.f;
				}
				CrystalWisdom = 3.f;
				CrystalStrength = 1.f;
				break;
			}
			case ECrystalColor::WHITE:
			{
				if (CrystalLevel % 3 == 0)
				{
					CrystalAgility = 1.f;
					CrystalStrength = 1.f;
				}
				CrystalWisdom = 3.f;
				CrystalEndurance = 2.f;
				break;
			}
			default: break;
			}
		}

		Hero.HeroAttributes.TotalCrystalsLevel++;
		Hero.HeroAttributes.Strength += Strength + CrystalStrength;
		Hero.HeroAttributes.Agility += Agility + CrystalAgility;
		Hero.HeroAttributes.Wisdom += Wisdom + CrystalWisdom;
		Hero.HeroAttributes.Endurance += Endurance + CrystalEndurance;
		Hero.HeroAttributes.CurrentHP = GetMaxHP(TempIndex);
		Hero.HeroAttributes.CurrentME = GetMaxME(TempIndex);
		TempIndex++;

		LevelUPData.Add(FLevelUPData(Strength, Agility, Wisdom, Endurance, CrystalStrength, CrystalAgility, CrystalWisdom, CrystalEndurance, CrystalColor, bNewSkill, SkillName, CrystalSprite));
	}
}

void UBLGameInstance::AddSkill(int32 HeroIndex, ECombatActionType ActionType, TSoftClassPtr<UBLAction> Skill)
{
	switch (ActionType)
	{
		case ECombatActionType::SPECIAL_SKILL:
		{
			if (!SaveGameData.HeroesData.Heroes.IsValidIndex(HeroIndex))
			{
				return;
			}

			SaveGameData.HeroesData.Heroes[HeroIndex].CombatActions.SpecialActions.Add(Skill);
			break;
		}
		default: break;

		//others if needed
	}
}

UPaperSprite* UBLGameInstance::GetRealHeroSprite(int32 HeroIndex)
{
	if (SaveGameData.HeroesData.Heroes.IsValidIndex(HeroIndex))
	{
		return SaveGameData.HeroesData.Heroes[HeroIndex].RealSprite;
	}
	return nullptr;
}


void UBLGameInstance::SaveGame()
{
	// I don't know why but sometimes if save slot exists and we try to overwrite it, editor crashes beceause of the operator = in TArray
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
	}

	SaveGameInstance = Cast<UBLSaveGame>(UGameplayStatics::CreateSaveGameObject(UBLSaveGame::StaticClass()));
	if (!IsValid(SaveGameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLGameInstance::SaveGame | SaveGameInstance is invalid"));
		return;
	}

	//Set Player location
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player)
	{
		SetPlayerLocationToSave(Player->GetActorLocation());
	}

	SaveGameData.MapName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));

	// Set game data in SaveGame object
	SaveGameInstance->SetSaveData(SaveGameData);
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
}

void UBLGameInstance::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGameInstance = Cast<UBLSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (!IsValid(SaveGameInstance))
		{
			UE_LOG(LogTemp, Warning, TEXT("UBLGameInstance::LoadGame | SaveGameInstance is invalid"));
			return;
		}		

		// Takes data from SaveGame object and assigns it to data in the game instance
		SaveGameInstance->GetSaveData(SaveGameData);

		// Set new location in PlayerCharacter BeginPlay
		bUseSavedPlayerLocation = true;

		APlayerCameraManager* CM = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		if (CM)
		{
			CM->StartCameraFade(0.f, 1.f, 1.f, FLinearColor(0.f, 0.f, 0.f), false, true);
			FTimerDelegate DelayDel;
			DelayDel.BindWeakLambda(this, [this]() { UGameplayStatics::OpenLevel(GetWorld(), SaveGameData.MapName); });
			FTimerHandle DelayTimer;
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 1.5f, false);
		}
		else
		{
			UGameplayStatics::OpenLevel(GetWorld(), SaveGameData.MapName);
		}
	}
}

void UBLGameInstance::ClearSaveSlot()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
	}

	SaveHeroData();
}

bool UBLGameInstance::IsSaveExist() const
{
	return UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
}

void UBLGameInstance::AddHero(const FHeroAssetInfo& NewHeroData)
{
	SaveGameData.HeroesData.Heroes.Add(NewHeroData);
}
