// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/NPC/NPCInteract/BLNPCInteract.h"
#include "BLNPCInteractDial.generated.h"

class UDialogueBaseComponent;

/**
 * NPC with which player can enter in interaction and start dialogue.
 */
UCLASS()
class BLADEOFLEGEND_API ABLNPCInteractDial : public ABLNPCInteract
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

#pragma region Interaction

public:
	// Overriding InteractionStart from InteractionInterface
	virtual void InteractionStart_Implementation(AActor* OtherInstigator) override;
#pragma endregion Interaction

#pragma region Dialogue

private:
	void InitializeDialogueComponent();

	UPROPERTY(EditDefaultsOnly, Category="BL|Dialogue")
	TSubclassOf<UDialogueBaseComponent> DialogueComponentClass;
	UPROPERTY(VisibleAnywhere, Category="BL|Dialogue")
	TObjectPtr<UDialogueBaseComponent> DialogueComponent;

#pragma endregion Dialogue
};
