// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLNPCInteractDial.h"
#include "BladeOfLegend/AREK/Components/Dialogue/DialogueBaseComponent.h"

void ABLNPCInteractDial::BeginPlay()
{
	Super::BeginPlay();

	InitializeDialogueComponent();
}

void ABLNPCInteractDial::InteractionStart_Implementation(AActor* OtherInstigator)
{
#pragma region NullChecks
	if (!DialogueComponent)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ABLNPCInteractDial::InteractionStart_Implementation|DialogueComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	Super::InteractionStart_Implementation(OtherInstigator);

	DialogueComponent->OpenConversation();
}

void ABLNPCInteractDial::InitializeDialogueComponent()
{
#pragma region NullChecks
	if (!DialogueComponentClass)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ABLNPCInteractDial::InitializeDialogueComponent|DialogueComponentClass is nullptr"))
		return;
	}
#pragma endregion NullChecks

	DialogueComponent = NewObject<UDialogueBaseComponent>(this, DialogueComponentClass);
	DialogueComponent->RegisterComponent();
}
