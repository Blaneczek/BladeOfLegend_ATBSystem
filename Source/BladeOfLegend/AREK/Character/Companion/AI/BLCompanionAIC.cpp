// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCompanionAIC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

ABLCompanionAIC::ABLCompanionAIC(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ABLCompanionAIC::SetFollowTarget(AActor* FollowTarget)
{
#pragma region NullChecks
	if (!FollowTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLCompanionAIC::SetFollowTarget|FollowTarget is nullptr"))
		return;
	}
#pragma endregion NullChecks

	GetBlackboardComponent()->SetValueAsObject(PlayerTargetKeyName, FollowTarget);
}

void ABLCompanionAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#pragma region NullChecks
	if (!CompanionBehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLCompanionAIC::OnPossess|CompanionBehaviorTree is nullptr"))
		return;
	}
#pragma endregion NullChecks

	RunBehaviorTree(CompanionBehaviorTree);
}
