// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actions/BLAction.h"
#include "BLBounceRangeAction.generated.h"

class ABLRangeProjectile;
class UPaperFlipbook;

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLBounceRangeAction : public UBLAction
{
	GENERATED_BODY()

public:
	virtual void ActivateAction(UBLActionComponent* OwnerComponent) override;
	virtual void ExecuteAction(ABLCombatSlot* Target) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TSubclassOf<ABLRangeProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UPaperFlipbook> ProjectileSprite;
};
