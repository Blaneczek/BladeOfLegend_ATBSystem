// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLGameModeMenu.h"
#include "BladeOfLegend/AREK/Menu/Options/Audio/BLSaveAudio.h"
#include "Kismet/GameplayStatics.h"

void ABLGameModeMenu::BeginPlay()
{
	Super::BeginPlay();

	LoadSaveAudio();
	UpdateGameSoundMix();
}

void ABLGameModeMenu::LoadSaveAudio()
{
	const bool bDoesSaveGameExist = UGameplayStatics::DoesSaveGameExist(SaveAudioSlotName, 0);

	if (bDoesSaveGameExist)
	{
		SaveAudio = Cast<UBLSaveAudio>(UGameplayStatics::LoadGameFromSlot(SaveAudioSlotName, 0));
	}
	else
	{
		SaveAudio = Cast<UBLSaveAudio>(UGameplayStatics::CreateSaveGameObject(UBLSaveAudio::StaticClass()));
	}
}

void ABLGameModeMenu::UpdateGameSoundMix() const
{
#pragma region NullChecks
	if (!SaveAudio)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLGameModeMenu::UpdateGameSoundMix|SaveAudio is nullptr"))
		return;
	}
	if (!GameSoundMix)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLGameModeMenu::UpdateGameSoundMix|GameSoundMix is nullptr"))
		return;
	}
	if (!MusicSoundClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLGameModeMenu::UpdateGameSoundMix|MusicSoundClass is nullptr"))
		return;
	}
	if (!SFXSoundClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLGameModeMenu::UpdateGameSoundMix|SFXSoundClass is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const float MusicVolumeValue = SaveAudio->GetMusicVolumeValue();
	UGameplayStatics::SetSoundMixClassOverride(this, GameSoundMix, MusicSoundClass, MusicVolumeValue);
	const float DialoguesVolumeValue = SaveAudio->GetDialogueVolumeValue();
	UGameplayStatics::SetSoundMixClassOverride(this, GameSoundMix, DialoguesSoundClass, DialoguesVolumeValue);
	const float SFXVolumeValue = SaveAudio->GetSFXVolumeValue();
	UGameplayStatics::SetSoundMixClassOverride(this, GameSoundMix, SFXSoundClass, SFXVolumeValue);
	const float UIVolumeValue = SaveAudio->GetUIVolumeValue();
	UGameplayStatics::SetSoundMixClassOverride(this, GameSoundMix, UISoundClass, UIVolumeValue);

	UGameplayStatics::PushSoundMixModifier(this, GameSoundMix);
}
