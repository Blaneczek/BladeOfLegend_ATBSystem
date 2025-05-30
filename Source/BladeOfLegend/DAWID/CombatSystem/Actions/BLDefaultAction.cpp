// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultAction.h"
#include "Characters/BLCombatCharacter.h"
#include "CombatComponents/BLActionComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"

void UBLDefaultAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (OwnerComponent && IsValid(OwnerChar))
	{
		OwnerComponent->DefaultAction();	
		OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
	}		
}

void UBLDefaultAction::ExecuteAction(ABLCombatSlot* Target)
{
	// In Default action type target is set to nullptr. Action is performed on OwnerChar instead
	if (!IsValid(OwnerChar))
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculations(nullptr, CombatManager);

	if (IsValid(ActionAnim))
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OnEndExecution.ExecuteIfBound();
	}
}
