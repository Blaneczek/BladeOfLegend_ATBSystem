// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLMultipleDefaultRangeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "CombatComponents/BLActionComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"

void UBLMultipleDefaultRangeAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (!OwnerComponent || !IsValid(OwnerChar))
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (IsValid(ActionAnim))
	{ 			
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, OwnerComponent](bool bResult) { OwnerComponent->MultipleDefaultRangeAction(ProjectileClass, ProjectileSprite); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OwnerComponent->MultipleDefaultRangeAction(ProjectileClass, ProjectileSprite);
	}

	OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
}

void UBLMultipleDefaultRangeAction::ExecuteAction(ABLCombatSlot* Target)
{
	if (!Target)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculations(Target, CombatManager);
	FTimerHandle Delay;
	FTimerDelegate DelayDel;
	DelayDel.BindWeakLambda(this, [this]() { OnEndExecution.ExecuteIfBound(); });
	GetWorld()->GetTimerManager().SetTimer(Delay, DelayDel, 0.1f, false);
}
