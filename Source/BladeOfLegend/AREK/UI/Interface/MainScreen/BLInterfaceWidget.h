// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLInterfaceWidget.generated.h"

class UBLGameInstance;
class UTextBase;
class UVerticalBox;
class UBLInterfaceCharacterSlot;
class UBLHeroDataAsset;

UCLASS()
class BLADEOFLEGEND_API UBLInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	void InitializeGameInstance();
	void InitializePlayerController();

	void SetCharacterSlots() const;
	void SetTeamInfo() const;

	UPROPERTY(VisibleAnywhere, Category="BL")
	TObjectPtr<UBLGameInstance> BLGameInstance;
	UPROPERTY(VisibleAnywhere, Category="BL")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UVerticalBox> VB_CharacterSlots;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget, AllowPrivateAccess=true), Category="BL")
	TObjectPtr<UTextBase> TB_Gold;

#pragma region ControlMenu

public:
	UFUNCTION(BlueprintCallable, Category="BL")
	void ToggleMenu();
	UFUNCTION(BlueprintCallable, Category="BL")
	void AddSubMenu(UUserWidget* SubMenu);
	UFUNCTION(BlueprintCallable, Category="BL")
	void RemoveSubMenu(UUserWidget* SubMenu);

private:
	void SetUIMode(const bool bUI) const;
	
	UPROPERTY(VisibleAnywhere, Category="BL")
	TArray<TObjectPtr<UUserWidget>> SubMenus;
	// todo create submenu c++ with back and exit logic
#pragma endregion ControlMenu
};
