// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/NPC/NPCInteract/BLNPCInteract.h"
#include "Data/BLHeroDataAsset.h"
#include "BLNPCCompanion.generated.h"

UCLASS()
class BLADEOFLEGEND_API ABLNPCCompanion : public ABLNPCInteract
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="BL|Companion")
	FHeroAssetInfo HeroInfo;
};
