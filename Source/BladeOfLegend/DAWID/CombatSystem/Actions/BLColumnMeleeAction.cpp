// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLColumnMeleeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "CombatComponents/BLActionComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"

void UBLColumnMeleeAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (OwnerComponent && IsValid(OwnerChar))
	{
		OwnerComponent->ColumnMeleeAction();
		OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
	}
}

void UBLColumnMeleeAction::ExecuteAction(const TArray<ABLCombatSlot*>& Targets)
{
	if (Targets.IsEmpty() || !IsValid(OwnerChar))
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (IsValid(ActionAnim))
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		ActionCalculationsMultiAtOnce(Targets, CombatManager);
	}
	else
	{
		ActionCalculationsMultiAtOnce(Targets, CombatManager);
		OnEndExecution.ExecuteIfBound();
	}
}
