// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BLSaveAudio.generated.h"

UCLASS()
class BLADEOFLEGEND_API UBLSaveAudio : public USaveGame
{
	GENERATED_BODY()

public:
	UBLSaveAudio(): MusicVolumeValue(DefaultVolumeValue), SFXVolumeValue(DefaultVolumeValue), DialogueVolumeValue(DefaultVolumeValue),
	                UIVolumeValue(DefaultVolumeValue)
	{
	}

	float GetMusicVolumeValue() const;
	void SetMusicVolumeValue(const float NewMusicValue);
	float GetSFXVolumeValue() const;
	void SetSFXVolumeValue(const float NewSFXValue);
	float GetDialogueVolumeValue() const;
	void SetDialogueVolumeValue(const float NewDialoguesValue);
	float GetUIVolumeValue() const;
	void SetUIVolumeValue(const float NewUIValue);

private:
	float DefaultVolumeValue{0.5f};

	UPROPERTY(VisibleAnywhere, Category="BL")
	float MusicVolumeValue;
	UPROPERTY(VisibleAnywhere, Category="BL")
	float SFXVolumeValue;
	UPROPERTY(VisibleAnywhere, Category="BL")
	float DialogueVolumeValue;
	UPROPERTY(VisibleAnywhere, Category="BL")
	float UIVolumeValue;
};
