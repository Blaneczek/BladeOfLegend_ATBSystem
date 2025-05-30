// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSpringArmDefault.h"

UBLSpringArmDefault::UBLSpringArmDefault()
{
	bDoCollisionTest = false;
}

void UBLSpringArmDefault::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SnapCameraToGridSize();
}

void UBLSpringArmDefault::SnapCameraToGridSize()
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const float GridSize = 1 / PixelPerUnit;
	const float NewLocationX = FMath::GridSnap(OwnerLocation.X, GridSize);
	const float NewLocationY = FMath::GridSnap(OwnerLocation.Y, GridSize);
	const FVector NewWorldLocation{NewLocationX, NewLocationY, 0.f};

	SetWorldLocation(NewWorldLocation);
}
