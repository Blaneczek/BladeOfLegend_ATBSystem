// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "DialogueBaseComponent.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "BladeOfLegend/AREK/Interfaces/Interaction/InteractionInterface.h"
#include "Components/AudioComponent.h"
#include "Engine/AssetManager.h"
#include "UI/DialogueWidget.h"

UDialogueBaseComponent::UDialogueBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
}

void UDialogueBaseComponent::OpenConversation()
{
	BeforeOpenConversation();

	InitializePlayerController();
	InitializeDialogueWidget();

#pragma region NullChecks
	if (!DialogueWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::OpenConversation|DialogueWidget is nullptr"))
		return;
	}
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::OpenConversation|PlayerController is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Bind AutoProgress so dialogue moves on when there is no option to choose
	DialogueWidget->OnAutoProgress.BindUObject(this, &UDialogueBaseComponent::AutoProgress);
	DialogueWidget->AddToViewport();

	const FInputModeUIOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);

	ProgressDialogue();
}

void UDialogueBaseComponent::InitializeDialogueWidget()
{
#pragma region NullChecks
	if (!DialogueWidgetClass)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UDialogueBaseComponent::InitializeDialogueWidget|DialogueWidgetClass is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Create DialogueWidget if there is no already set one
	if (!DialogueWidget)
	{
		DialogueWidget = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetClass);
		DialogueWidget->SetOwningDialogueComponent(this);
	}
}

void UDialogueBaseComponent::ProgressDialogue()
{
	// Resets DialogueIndex
	DialogueIndex = -1;

	Dialogue();
}

void UDialogueBaseComponent::CloseConversation() const
{
#pragma region NullChecks
	if (!DialogueWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::CloseConversation|DialogueWidget is nullptr"))
		return;
	}
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::CloseConversation|AudioComponent is nullptr"))
		return;
	}
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::CloseConversation|PlayerController is nullptr"))
		return;
	}
#pragma endregion NullChecks

	DialogueWidget->RemoveFromParent();
	AudioComponent->Stop();

	const FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(false);

	const UPaperZDAnimationComponent* AnimationComponent = Cast<UPaperZDAnimationComponent>(
		GetOwner()->GetComponentByClass(UPaperZDAnimationComponent::StaticClass()));

#pragma region NullChecks
	if (!AnimationComponent)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UDialogueBaseComponent::CloseConversation|AnimationComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	UPaperZDAnimInstance* AnimInstance = AnimationComponent->GetAnimInstance();

#pragma region NullChecks
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::CloseConversation|AnimInstance is nullptr"))
		return;
	}
#pragma endregion NullChecks

	AnimInstance->StopAllAnimationOverrides();

	AActor* Owner = GetOwner();
	// If owner has InteractionInterface implemented fire InteractionEnd method
	const bool bImplementsInterface = Owner->Implements<UInteractionInterface>();
	if (bImplementsInterface)
	{
		IInteractionInterface::Execute_InteractionEnd(Owner);
	}
}

int32 UDialogueBaseComponent::AddDialogue(EDialogueState& OutState, const FText Dialogue,
                                          const FText Speaker,
                                          const TArray<FText> OptionsText,
                                          const TSoftObjectPtr<USoundBase> DialogueAudio,
                                          const TSoftObjectPtr<UPaperZDAnimSequence> AnimSequence)
{
#pragma region NullChecks
	if (!DialogueWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::AddDialogue|DialogueWidget is nullptr"))
		return -1;
	}
#pragma endregion NullChecks

	// Increments DialogueIndex, so component would know on what dialogue we are
	DialogueIndex++;

	// If there is already dialogue with this DialogueIndex in DialogueProgress,
	// return PassThrough and return selected earlier Option
	const int32* OptionIndexPtr = DialogueProgress.Find(DialogueIndex);
	if (OptionIndexPtr)
	{
		OutState = EDialogueState::PassThrough;
		return *OptionIndexPtr;
	}

	// If there is no dialogue with this DialogueIndex in DialogueProgress,
	// Update dialogue widget with speaker, dialogue and options
	// todo add audio to optionstext
	DialogueWidget->UpdateDialogueText(DialogueIndex, Speaker, Dialogue, OptionsText);
	// Play Audio if set
	PlayDialogueAudio(DialogueAudio);
	// Play Animation if set
	PlayAnimation(AnimSequence);

	// return Updated and return 0, because there can't be already chosen option first time widget appears
	OutState = EDialogueState::Updated;
	return 0;
}

void UDialogueBaseComponent::UpdateSelectedOption(const int32 ProgressIndex, const int32 OptionIndex)
{
	// Update DialogueProgress map with index of added dialogue and answer option index
	DialogueProgress.Add(ProgressIndex, OptionIndex);

	ProgressDialogue();
}

void UDialogueBaseComponent::ClearDialogueProgress()
{
	DialogueProgress.Empty();
}

void UDialogueBaseComponent::RemoveDialogueProgress(const int32 ProgressSteps)
{
	for (int32 i = 0; i < ProgressSteps; i++)
	{
		DialogueProgress.Remove(DialogueIndex - i);
	}
}

void UDialogueBaseComponent::SetDialogueBranch(const FName DialogueBranch,
                                               const EDialogueProgressStoreType ProgressHandler)
{
	switch (ProgressHandler)
	{
	// Saves current dialogue branch in BranchProgress
	case EDialogueProgressStoreType::Update:
		{
			FDialogueProgress StructDialogueProgress;
			StructDialogueProgress.Progress = DialogueProgress;
			BranchProgress.Add(ActiveDialogueBranch, StructDialogueProgress);
			break;
		}
	// If current dialogue branch is in BranchProgress removes it
	case EDialogueProgressStoreType::Clear:
		{
			BranchProgress.Remove(ActiveDialogueBranch);
			break;
		}
	}

	// If there is already saved branch progress of give dialogue branch name
	// update dialogue progress
	const FDialogueProgress* FoundProgress = BranchProgress.Find(DialogueBranch);
	if (FoundProgress)
	{
		DialogueProgress = FoundProgress->Progress;
	}
	ActiveDialogueBranch = DialogueBranch;
}

void UDialogueBaseComponent::AutoProgress()
{
	UpdateSelectedOption(DialogueIndex, 0);
}

FName UDialogueBaseComponent::GetActiveDialogueBranch() const
{
	return ActiveDialogueBranch;
}

void UDialogueBaseComponent::PlayDialogueAudio(const TSoftObjectPtr<USoundBase>& SoundBaseSoftPtr)
{
	// Check if SoundBaseSoftPtr is set
	if (!SoundBaseSoftPtr.IsNull())
	{
		// Check if SoundBaseSoftPtr is loaded
		if (SoundBaseSoftPtr.IsValid())
		{
			// If yes play sound
			PlayLoadedAudio(SoundBaseSoftPtr.Get());
		}
		else
		{
			// If not request async load
			LoadDialogueAudio(SoundBaseSoftPtr);
		}
	}
}

void UDialogueBaseComponent::LoadDialogueAudio(TSoftObjectPtr<USoundBase> SoundBaseSoftPtr)
{
	// Get async loading manager
	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	// Request asynchronous loading, when loaded fires OnAudioLoaded function
	Manager.RequestAsyncLoad(SoundBaseSoftPtr.ToSoftObjectPath(),
	                         FStreamableDelegate::CreateUObject(
		                         this, &UDialogueBaseComponent::OnAudioLoaded,
		                         SoundBaseSoftPtr));
}

// ReSharper disable once CppPassValueParameterByConstReference
void UDialogueBaseComponent::OnAudioLoaded(TSoftObjectPtr<USoundBase> SoundBaseSoftPtr) const
{
	PlayLoadedAudio(SoundBaseSoftPtr.Get());
}

void UDialogueBaseComponent::PlayLoadedAudio(const TObjectPtr<USoundBase> Audio) const
{
#pragma region NullChecks
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::PlayLoadedAudio|AudioComponent is nullptr"))
		return;
	}
	if (!Audio)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::PlayLoadedAudio|Audio is nullptr"))
		return;
	}
#pragma endregion NullChecks

	AudioComponent->SetSound(Audio);
	AudioComponent->Play();
}

void UDialogueBaseComponent::PlayAnimation(const TSoftObjectPtr<UPaperZDAnimSequence>& AnimSequenceSoftPtr)
{
	ActiveAnimSequenceSoftPtr = AnimSequenceSoftPtr;

	// Check if AnimSequenceSoftPtr is set
	if (!ActiveAnimSequenceSoftPtr.IsNull())
	{
		// Check if ActiveAnimSequenceSoftPtr is loaded
		if (AnimSequenceSoftPtr.IsValid())
		{
			// If yes play animation
			PlayLoadedAnimationOverride(ActiveAnimSequenceSoftPtr.Get());
		}
		else
		{
			// If not request async load
			LoadDialogueAnimation(ActiveAnimSequenceSoftPtr);
		}
	}
	// If AnimSequenceSoftPtr is not set and there are some generic anims choose one
	else if (!GenericTalkingAnimSequencesSet.IsEmpty())
	{
		AssignGenericAnimSequence();
	}
}

void UDialogueBaseComponent::AssignGenericAnimSequence()
{
	// Convert set to array
	TArray<TSoftObjectPtr<UPaperZDAnimSequence>> GenericTalkingAnimSequencesArr = GenericTalkingAnimSequencesSet.
		Array();
	// Get random element of array
	ActiveAnimSequenceSoftPtr = GenericTalkingAnimSequencesArr[FMath::RandHelper(
		GenericTalkingAnimSequencesArr.Num())];
}

void UDialogueBaseComponent::LoadDialogueAnimation(TSoftObjectPtr<UPaperZDAnimSequence> AnimSequenceSoftPtr)
{
	// Get async loading manager
	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	// Request asynchronous loading, when loaded it will fire OnAnimationLoaded function
	Manager.RequestAsyncLoad(AnimSequenceSoftPtr.ToSoftObjectPath(),
	                         FStreamableDelegate::CreateUObject(
		                         this, &UDialogueBaseComponent::OnAnimationLoaded,
		                         AnimSequenceSoftPtr));
}

// ReSharper disable once CppPassValueParameterByConstReference
void UDialogueBaseComponent::OnAnimationLoaded(TSoftObjectPtr<UPaperZDAnimSequence> AnimSequenceSoftPtr) const
{
	PlayLoadedAnimationOverride(AnimSequenceSoftPtr.Get());
}

void UDialogueBaseComponent::PlayLoadedAnimationOverride(const TObjectPtr<UPaperZDAnimSequence> AnimSequence) const
{
	const UPaperZDAnimationComponent* AnimationComponent = Cast<UPaperZDAnimationComponent>(
		GetOwner()->GetComponentByClass(UPaperZDAnimationComponent::StaticClass()));

#pragma region NullChecks
	if (!AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDialogueBaseComponent::PlayLoadedAnimationOverride|AnimSequence is nullptr"))
		return;
	}
	if (!AnimationComponent)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UDialogueBaseComponent::PlayLoadedAnimationOverride|AnimationComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	UPaperZDAnimInstance* AnimInstance = AnimationComponent->GetAnimInstance();
	// PlayAnimationOverride on DefaultSlot
	AnimInstance->PlayAnimationOverride(AnimSequence);
}

void UDialogueBaseComponent::InitializePlayerController()
{
	PlayerController = GetWorld()->GetFirstPlayerController();
}
