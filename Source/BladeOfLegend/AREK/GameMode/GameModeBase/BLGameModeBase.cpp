// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ABLGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerCameraManager* CM = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CM)
	{
		CM->StartCameraFade(1.f, 0.f, 1.5f, FLinearColor(0.f, 0.f, 0.f), false, true);
	}
}
