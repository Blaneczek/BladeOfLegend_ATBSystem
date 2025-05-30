// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSaveAudio.h"

float UBLSaveAudio::GetMusicVolumeValue() const
{
	return MusicVolumeValue;
}

void UBLSaveAudio::SetMusicVolumeValue(const float NewMusicValue)
{
	MusicVolumeValue = NewMusicValue;
}

float UBLSaveAudio::GetSFXVolumeValue() const
{
	return SFXVolumeValue;
}

void UBLSaveAudio::SetSFXVolumeValue(const float NewSFXValue)
{
	SFXVolumeValue = NewSFXValue;
}

float UBLSaveAudio::GetDialogueVolumeValue() const
{
	return DialogueVolumeValue;
}

void UBLSaveAudio::SetDialogueVolumeValue(const float NewDialoguesValue)
{
	DialogueVolumeValue = NewDialoguesValue;
}

float UBLSaveAudio::GetUIVolumeValue() const
{
	return UIVolumeValue;
}

void UBLSaveAudio::SetUIVolumeValue(const float NewUIValue)
{
	UIVolumeValue = NewUIValue;
}
