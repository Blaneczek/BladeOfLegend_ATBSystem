// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLWorldItem.generated.h"

class UPaperSprite;
class UBoxComponent;
class UPaperSpriteComponent;

UCLASS()
class BLADEOFLEGEND_API ABLWorldItem : public AActor
{
	GENERATED_BODY()

public:
	ABLWorldItem();

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> TranslucencyHelper;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPaperSpriteComponent> SpriteComponent;
};
