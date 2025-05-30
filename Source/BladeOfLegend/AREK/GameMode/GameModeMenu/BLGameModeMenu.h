// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BLGameModeMenu.generated.h"

class UBLSaveAudio;

UCLASS()
class BLADEOFLEGEND_API ABLGameModeMenu : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

#pragma region SaveAudio

private:
	void LoadSaveAudio();
	void UpdateGameSoundMix() const;

	UPROPERTY(VisibleAnywhere, Category="BL|Save|Audio")
	FString SaveAudioSlotName{"SaveAudio"};
	UPROPERTY(VisibleAnywhere, Category="BL|Save|Audio")
	TObjectPtr<UBLSaveAudio> SaveAudio;

	UPROPERTY(EditDefaultsOnly, Category="BL|Save|Audio")
	TObjectPtr<USoundMix> GameSoundMix;
	UPROPERTY(EditDefaultsOnly, Category="BL|Save|Audio")
	TObjectPtr<USoundClass> MusicSoundClass;
	UPROPERTY(EditDefaultsOnly, Category="BL|Save|Audio")
	TObjectPtr<USoundClass> DialoguesSoundClass;
	UPROPERTY(EditDefaultsOnly, Category="BL|Save|Audio")
	TObjectPtr<USoundClass> SFXSoundClass;
	UPROPERTY(EditDefaultsOnly, Category="BL|Save|Audio")
	TObjectPtr<USoundClass> UISoundClass;

#pragma endregion SaveAudio
};
