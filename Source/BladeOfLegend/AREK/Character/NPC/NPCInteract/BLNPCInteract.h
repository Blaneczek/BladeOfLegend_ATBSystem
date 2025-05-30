// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/NPC/BLNPC.h"
#include "BladeOfLegend/AREK/Interfaces/Interaction/InteractionInterface.h"
#include "BLNPCInteract.generated.h"

class UWidgetComponent;
class UDialogueBaseComponent;
class UTextRenderComponent;

/**
 * NPC with which player can enter in interaction.
 */
UCLASS()
class BLADEOFLEGEND_API ABLNPCInteract : public ABLNPC, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ABLNPCInteract();

	virtual void BeginPlay() override;

#pragma region NameLabel

private:
	void SetTextComponentWithCharacterName() const;

	// TextComponent for CharacterName
	UPROPERTY(EditDefaultsOnly, Category="BL|Name")
	TObjectPtr<UTextRenderComponent> NameTextComponent;

#pragma endregion NameLabel

#pragma region Interaction

public:
	// Overriding InteractionStart from InteractionInterface
	virtual void InteractionStart_Implementation(AActor* OtherInstigator) override;
	// Overriding SetInteractableState from InteractionInterface
	virtual void SetInteractableState_Implementation(bool bCanShowInteract) override;
	// Overriding InteractionEnd from InteractionInterface
	virtual void InteractionEnd_Implementation() override;
	// Overriding GetInteractionInstigator from InteractionInterface
	virtual AActor* GetInteractionInstigator_Implementation() override;
	// Overriding SetIsInInteractSphere from InteractionInterface
	virtual void SetIsInInteractSphere_Implementation(bool bIsInInteractSphere) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="BL|Interaction")
	TObjectPtr<AActor> InteractInstigator;

private:
	// WidgetComponent for interact key
	UPROPERTY(EditDefaultsOnly, Category="BL|Interaction")
	TObjectPtr<UWidgetComponent> InteractKeyWidgetComponent;
#pragma endregion Interaction
};
