// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/NPC/BLNPC.h"
#include "BLNPCDial.generated.h"

class UDialogueBaseComponent;

/**
 * NPC with dialogue component.
 */
UCLASS()
class BLADEOFLEGEND_API ABLNPCDial : public ABLNPC
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

#pragma region Dialogue

public:
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	void OpenConversation() const;

private:
	void CreateDialogueComponent();
	UFUNCTION(BlueprintCallable, Category="BL|Dialogue")
	UDialogueBaseComponent* GetDialogueComponent() const;

	UPROPERTY(EditDefaultsOnly, Category="BL|Dialogue")
	TSubclassOf<UDialogueBaseComponent> DialogueComponentClass;
	UPROPERTY(BlueprintReadOnly, Category="BL|Dialogue", meta=(AllowPrivateAccess=true))
	TObjectPtr<UDialogueBaseComponent> DialogueComponent;

#pragma endregion Dialogue
};
