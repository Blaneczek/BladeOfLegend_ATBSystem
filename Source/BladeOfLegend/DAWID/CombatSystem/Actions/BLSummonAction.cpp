// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSummonAction.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Characters/BLCombatCharacter.h"
#include "CombatComponents/BLActionComponent.h"
#include "Core/BLCombatSlot.h"

void UBLSummonAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (!OwnerComponent || !IsValid(OwnerChar))
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (IsValid(ActionAnim))
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, OwnerComponent](bool bResult) { OwnerComponent->MultipleInPlaceAction(EffectClass); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OwnerComponent->MultipleInPlaceAction(EffectClass);
	}
}

void UBLSummonAction::ExecuteAction(const TArray<ABLCombatSlot*>& Targets)
{
	if (Targets.IsEmpty() || EnemiesToSpawn.IsEmpty())
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}
	
	for (auto& Slot : Targets)
	{
		if (Slot)
		{
			// If there is already a character in the slot, destroy him first.
			Slot->DestroyCharacter();
			const int32 Random = FMath::RandRange(0, EnemiesToSpawn.Num() - 1);
			Slot->SpawnEnemy(EnemiesToSpawn[Random].BaseData, EnemiesToSpawn[Random].Level, EnemiesToSpawn[Random].Actions, false);
			ActionCalculations(Slot, CombatManager);
		}
	}
	OnEndExecution.ExecuteIfBound();
}
