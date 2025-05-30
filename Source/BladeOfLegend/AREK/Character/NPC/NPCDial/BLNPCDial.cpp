// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLNPCDial.h"
#include "BladeOfLegend/AREK/Components/Dialogue/DialogueBaseComponent.h"

void ABLNPCDial::BeginPlay()
{
	CreateDialogueComponent();
	Super::BeginPlay();
}

void ABLNPCDial::OpenConversation() const
{
#pragma region NullChecks
	if (!DialogueComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLNPCDial::OpenConversation|DialogueComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	DialogueComponent->OpenConversation();
}

void ABLNPCDial::CreateDialogueComponent()
{
#pragma region NullChecks
	if (!DialogueComponentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLNPCDial::InitializeDialogueComponent|DialogueComponentClass is nullptr"))
		return;
	}
#pragma endregion NullChecks

	DialogueComponent = NewObject<UDialogueBaseComponent>(this, DialogueComponentClass);
	DialogueComponent->RegisterComponent();
}

UDialogueBaseComponent* ABLNPCDial::GetDialogueComponent() const
{
	if (!DialogueComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLNPCDial::GetDialogueComponent|DialogueComponent is nullptr"))
		return nullptr;
	}

	return DialogueComponent;
}
