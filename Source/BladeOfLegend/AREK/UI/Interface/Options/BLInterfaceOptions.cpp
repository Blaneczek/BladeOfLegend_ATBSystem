// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLInterfaceOptions.h"
#include "BladeOfLegend/AREK/Menu/Options/Audio/BLSaveAudio.h"
#include "BladeOfLegend/AREK/Menu/Options/UI/BLMenuOptionsWidget.h"
#include "BladeOfLegend/AREK/UI/TextBase.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"

void UBLInterfaceOptions::NativeConstruct()
{
	Super::NativeConstruct();

	LoadSaveAudio();
	UpdateSliders();
}

void UBLInterfaceOptions::LoadSaveAudio()
{
	const bool bDoesSaveGameExist{UGameplayStatics::DoesSaveGameExist(SaveAudioSlotName, 0)};

	if (bDoesSaveGameExist)
	{
		SaveAudio = Cast<UBLSaveAudio>(UGameplayStatics::LoadGameFromSlot(SaveAudioSlotName, 0));
	}
	else
	{
		SaveAudio = Cast<UBLSaveAudio>(UGameplayStatics::LoadGameFromSlot(SaveAudioSlotName, 0));
	}
}

void UBLInterfaceOptions::UpdateSaveAudio(const float Value, const ESoundClassType SoundClassType) const
{
#pragma region NullChecks
	if (!SaveAudio)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::UpdateSaveAudio|SaveAudio is nullptr"))
		return;
	}
#pragma endregion NullChecks

	switch (SoundClassType)
	{
	case ESoundClassType::Music:
		{
			SaveAudio->SetMusicVolumeValue(Value);
			break;
		}
	case ESoundClassType::Dialogues:
		{
			SaveAudio->SetDialogueVolumeValue(Value);
			break;
		}
	case ESoundClassType::SFX:
		{
			SaveAudio->SetSFXVolumeValue(Value);
			break;
		}
	case ESoundClassType::UI:
		{
			SaveAudio->SetUIVolumeValue(Value);
			break;
		}
	}
	UGameplayStatics::SaveGameToSlot(SaveAudio, SaveAudioSlotName, 0);
}

void UBLInterfaceOptions::OnSliderValueChanged(const float Value, USoundMix* SoundMix, USoundClass* SoundClass,
                                               UTextBase* TB_VolumeValue, ESoundClassType SoundClassType) const
{
#pragma region NullChecks
	if (!SoundMix)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::OnSliderValueChanged|GameSoundMix is nullptr"))
		return;
	}
	if (!SoundClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::OnSliderValueChanged|SoundClass is nullptr"))
		return;
	}
	if (!TB_VolumeValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::OnSliderValueChanged|TB_VolumeValue is nullptr"))
		return;
	}
#pragma endregion NullChecks

	UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, SoundClass, Value);
	UGameplayStatics::PushSoundMixModifier(this, SoundMix);

	UpdateSaveAudio(Value, SoundClassType);

	const int32 ValueInt = FMath::RoundToInt(Value * 100.f);
	const FText ValueText = FText::AsNumber(ValueInt);
	TB_VolumeValue->SetTextText(ValueText);
}

void UBLInterfaceOptions::UpdateSliders() const
{
#pragma region NullChecks
	if (!SaveAudio)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::UpdateSliders|SaveAudio is nullptr"))
		return;
	}
	if (!MusicSlider)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::UpdateSliders|MusicSlider is nullptr"))
		return;
	}
	if (!DialoguesSlider)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::UpdateSliders|DialoguesSlider is nullptr"))
		return;
	}
	if (!SFXSlider)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::UpdateSliders|SFXSlider is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Set slider values from save audio
	const float MusicVolumeValue{SaveAudio->GetMusicVolumeValue()};
	MusicSlider->SetValue(MusicVolumeValue);

	const float DialoguesVolumeValue{SaveAudio->GetDialogueVolumeValue()};
	DialoguesSlider->SetValue(DialoguesVolumeValue);

	const float SFXVolumeValue{SaveAudio->GetSFXVolumeValue()};
	SFXSlider->SetValue(SFXVolumeValue);

	const float UIVolumeValue{SaveAudio->GetUIVolumeValue()};
	UISlider->SetValue(UIVolumeValue);
}

void UBLInterfaceOptions::OnVolumeButtonClicked(USlider* VolumeSlider, EVolumeAdjustment VolumeAdjustment)
{
#pragma region NullChecks
	if (!VolumeSlider)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLInterfaceOptions::OnVolumeButtonClicked|VolumeSlider is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const float VolumeValue = VolumeSlider->GetValue();

	switch (VolumeAdjustment)
	{
	case EVolumeAdjustment::Increase:
		{
			VolumeSlider->SetValue(VolumeValue >= 1.f - VolumeSteps ? 1.f : VolumeValue + VolumeSteps);
			break;
		}
	case EVolumeAdjustment::Decrease:
		{
			VolumeSlider->SetValue(VolumeValue <= VolumeSteps ? 0.f : VolumeValue - VolumeSteps);
			break;
		}
	}
}
