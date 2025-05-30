// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "BLSpringArmDefault.generated.h"

UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLSpringArmDefault : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UBLSpringArmDefault();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Pixel Perfect camera setting
	void SnapCameraToGridSize();

	float PixelPerUnit{1.f};
};
