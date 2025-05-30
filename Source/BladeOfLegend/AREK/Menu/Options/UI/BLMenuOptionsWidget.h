// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLMenuOptionsWidget.generated.h"

class UTextBase;
class UBLSaveAudio;
class USlider;

UENUM()
enum class EVolumeAdjustment : uint8
{
	Increase,
	Decrease
};

UENUM()
enum class ESoundClassType : uint8
{
	Music,
	Dialogues,
	SFX,
	UI
};

UCLASS()
class BLADEOFLEGEND_API UBLMenuOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

#pragma region AudioSave

private:
	void LoadSaveAudio();
	void UpdateSaveAudio(const float Value, const ESoundClassType SoundClassType) const;

	UPROPERTY(VisibleAnywhere, Category="BL")
	FString SaveAudioSlotName{"SaveAudio"};
	UPROPERTY(VisibleAnywhere, Category="BL")
	TObjectPtr<UBLSaveAudio> SaveAudio;
#pragma endregion AudioSave

#pragma region AudioSliders

private:
	UFUNCTION(BlueprintCallable, Category="BL")
	void OnSliderValueChanged(const float Value, USoundMix* SoundMix, USoundClass* SoundClass,
	                          UTextBase* TB_VolumeValue, ESoundClassType SoundClassType) const;

	void UpdateSliders() const;

	UPROPERTY(BlueprintReadOnly, Category="BL", meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<USlider> MusicSlider;
	UPROPERTY(BlueprintReadOnly, Category="BL", meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<USlider> DialoguesSlider;
	UPROPERTY(BlueprintReadOnly, Category="BL", meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<USlider> SFXSlider;
	UPROPERTY(BlueprintReadOnly, Category="BL", meta=(BindWidget, AllowPrivateAccess=true))
	TObjectPtr<USlider> UISlider;
#pragma endregion AudioSliders

#pragma region AudioButtons

private:
	UFUNCTION(BlueprintCallable, Category="BL")
	void OnVolumeButtonClicked(USlider* VolumeSlider, EVolumeAdjustment VolumeAdjustment);

	UPROPERTY(VisibleAnywhere, Category="BL")
	float VolumeSteps{0.1f};
#pragma endregion AudioButtons
};
