// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/BLCombatUtilities.h"
#include "BLGameInstance.generated.h"

class UBLHeroDataAsset;
class UBLSaveGame;

UCLASS()
class BLADEOFLEGEND_API UBLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SetCombatData(const FCombatData& InCombatData, const FPostCombatData& InPostCombatData);

	UFUNCTION(BlueprintCallable)
	void SetEnemyFlag(const FName& EnemyTag, bool bDead);

	UFUNCTION(BlueprintCallable)
	const TMap<FName, bool>& GetEnemies() const { return SaveGameData.Enemies; }

	UFUNCTION(BlueprintCallable)
	void SaveHeroData();

	UFUNCTION(BlueprintCallable)
	void LoadHeroData();

	UFUNCTION(BlueprintCallable)
	FSaveGameData& GetSaveGameData() { return SaveGameData; }

	UFUNCTION(BlueprintCallable)
	const FSaveGameData& GetSaveGameDataConst() const { return SaveGameData; }

	TSoftObjectPtr<UWorld> GetPostCombatMapName() const { return PostCombatData.PostCombatMap; }

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	bool UseSavedPlayerLocation() const { return bUseSavedPlayerLocation; }
	
	UFUNCTION(BlueprintCallable)
	bool IsEndGame() const { return bEndGame; }

	UFUNCTION(BlueprintCallable)
	void SetUseSavedPlayerLocation(bool bUse) { bUseSavedPlayerLocation = bUse; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerLocationToSave(const FVector& Location) { SaveGameData.PlayerLocation = Location; }

	UFUNCTION(BlueprintCallable)
	bool UseCombatPlayerLocation() const { return PostCombatData.bUseNewPlayerPosition; }

	UFUNCTION(BlueprintCallable)
	void SetUseCombatPlayerLocation(bool bUse) { PostCombatData.bUseNewPlayerPosition = bUse; }

	UFUNCTION(BlueprintCallable)
	void ClearSaveSlot();

	UFUNCTION(BlueprintCallable)
	bool IsSaveExist() const;

	UFUNCTION(BlueprintCallable)
	void AddHero(const FHeroAssetInfo& NewHeroData);

	UFUNCTION(BlueprintCallable)
	const FVector& GetNewPostCombatLocation() const { return PostCombatData.PlayerPosition; }

	UFUNCTION(BlueprintCallable)
	const FVector& GetSavedLocation() const { return SaveGameData.PlayerLocation; }

	/*************************/
	/** Takes data from the hero (stats) and calculates the variables needed for combat */
	UFUNCTION(BlueprintCallable)
	FCombatCharData CalculateBaseCombatData(int32 Index);

	/** Adds to data asset reward from won combat */
	void AddCombatReward(int32 InExperience, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems);

	UFUNCTION(BlueprintCallable)
	float GetMaxHP(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	float GetMaxME(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void LevelUP();

	const TArray<FLevelUPData>& GetLevelUPData() const { return LevelUPData; }

	bool IsLevelUP() const { return bLevelUP; }

	UFUNCTION(BlueprintCallable)
	void AddSkill(int32 HeroIndex, ECombatActionType ActionType, TSoftClassPtr<UBLAction> Skill);

	UFUNCTION(BlueprintCallable)
	UPaperSprite* GetRealHeroSprite(int32 HeroIndex);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UBLAction> SpecialSkillKrolth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UBLAction> SpecialSkillCardie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatData CombatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPostCombatData PostCombatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UBLHeroDataAsset> HeroDataAsset;

	/** Saved in save system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSaveGameData SaveGameData;

	UPROPERTY()
	FString SaveSlotName = FString(TEXT("MainSaveSlot"));

	UPROPERTY()
	TObjectPtr<UBLSaveGame> SaveGameInstance;

	UPROPERTY()
	bool bUseSavedPlayerLocation = false;


	UPROPERTY()
	TArray<FLevelUPData> LevelUPData;

	UPROPERTY()
	bool bLevelUP = false;

	UPROPERTY()
	bool bEndGame = false;
};
