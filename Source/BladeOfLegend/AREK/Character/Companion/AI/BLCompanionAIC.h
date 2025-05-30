// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BLCompanionAIC.generated.h"

UCLASS()
class BLADEOFLEGEND_API ABLCompanionAIC : public AAIController
{
	GENERATED_BODY()

public:
	ABLCompanionAIC(const FObjectInitializer& ObjectInitializer);

	void SetFollowTarget(AActor* FollowTarget);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="BL")
	TObjectPtr<UBehaviorTree> CompanionBehaviorTree;
	UPROPERTY(EditDefaultsOnly, Category="BL")
	FName PlayerTargetKeyName{"PlayerTarget"};
};
