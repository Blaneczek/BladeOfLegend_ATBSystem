// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "DialogueBaseComponent.generated.h"

class UDialogueWidget;

UENUM()
enum class EDialogueState : uint8
{
	PassThrough,
	Updated
};

UENUM()
enum class EDialogueProgressStoreType : uint8
{
	Update,
	Clear
};

USTRUCT()
struct FDialogueProgress
{
	GENERATED_BODY()

	TMap<int32, int32> Progress;
};

class UPaperZDAnimationComponent;
class UPaperZDAnimSequence;

UCLASS(Abstract, Blueprintable)
class BLADEOFLEGEND_API UDialogueBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDialogueBaseComponent();

	void OpenConversation();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="BL|Dialogue")
	void Dialogue();
	UFUNCTION(BlueprintImplementableEvent, Category="BL|Dialogue")
	void BeforeOpenConversation();

private:
	void InitializeDialogueWidget();

	void ProgressDialogue();
	// Removes widgets, stop any audio and any animation
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	void CloseConversation() const;
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue", meta=(ExpandEnumAsExecs="OutState"))
	UPARAM(DisplayName="OptionIndex")
	int32 AddDialogue(EDialogueState& OutState, const FText Dialogue,
	                  const FText Speaker,
	                  const TArray<FText> OptionsText,
	                  const TSoftObjectPtr<USoundBase> DialogueAudio,
	                  const TSoftObjectPtr<UPaperZDAnimSequence> AnimSequence);
	// Fires on auto progress or on clicked widget answer option
	// Updates dialogue progress
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	void UpdateSelectedOption(const int32 ProgressIndex, const int32 OptionIndex);
	// Fully resets dialogue progress
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	void ClearDialogueProgress();
	// Go back in the dialogue by a specific number of steps
	// Use when you want to start dialogue in s
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	void RemoveDialogueProgress(const int32 ProgressSteps);
	// Switch between dialogue branches,
	// and decide whether it should save current dialogue branch progress or clear it
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue", meta=(ExpandEnumAsExecs="ProgressHandler"))
	void SetDialogueBranch(const FName DialogueBranch, const EDialogueProgressStoreType ProgressHandler);
	// Fires if there is no answer options to choose
	UFUNCTION()
	void AutoProgress();

	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	FName GetActiveDialogueBranch() const;

	UPROPERTY(EditDefaultsOnly, Category="BL|Dialogue|Widget")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;

	UPROPERTY(VisibleAnywhere, Category="BL|Dialogue")
	TMap<int32, int32> DialogueProgress;
	UPROPERTY(VisibleAnywhere, Category="BL|Dialogue")
	int32 DialogueIndex{0};

	UPROPERTY(VisibleAnywhere, Category="BL|Dialogue")
	TMap<FName, FDialogueProgress> BranchProgress;
	UPROPERTY(VisibleAnywhere, Category="BL|Dialogue")
	FName ActiveDialogueBranch{"Default"};

#pragma region Audio

private:
	void PlayDialogueAudio(const TSoftObjectPtr<USoundBase>& SoundBaseSoftPtr);

	// Async loading audio
	// todo unload audio
	void LoadDialogueAudio(TSoftObjectPtr<USoundBase> SoundBaseSoftPtr);
	void OnAudioLoaded(TSoftObjectPtr<USoundBase> SoundBaseSoftPtr) const;
	void PlayLoadedAudio(const TObjectPtr<USoundBase> Audio) const;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

#pragma endregion Audio

#pragma region Animation

private:
	void PlayAnimation(const TSoftObjectPtr<UPaperZDAnimSequence>& AnimSequenceSoftPtr);
	void AssignGenericAnimSequence();

	// Async loading animation
	// todo unload animation
	void LoadDialogueAnimation(TSoftObjectPtr<UPaperZDAnimSequence> AnimSequenceSoftPtr);
	void OnAnimationLoaded(TSoftObjectPtr<UPaperZDAnimSequence> AnimSequenceSoftPtr) const;
	void PlayLoadedAnimationOverride(const TObjectPtr<UPaperZDAnimSequence> AnimSequence) const;

	UPROPERTY()
	TSoftObjectPtr<UPaperZDAnimSequence> ActiveAnimSequenceSoftPtr;
	UPROPERTY()
	TSet<TSoftObjectPtr<UPaperZDAnimSequence>> GenericTalkingAnimSequencesSet;

#pragma  endregion Animation

#pragma region PlayerController

private:
	void InitializePlayerController();

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

#pragma endregion PlayerController
};
