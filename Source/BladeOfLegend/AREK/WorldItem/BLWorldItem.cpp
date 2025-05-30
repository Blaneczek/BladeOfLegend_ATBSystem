// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLWorldItem.h"
#include "Components/BoxComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"

ABLWorldItem::ABLWorldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create BoxComponent helpers for translucency
	TranslucencyHelper = CreateDefaultSubobject<UBoxComponent>(TEXT("TranslucencyHelper"));
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));

#pragma region NullChecks
	if (!TranslucencyHelper)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLWorldItem::ABLWorldItem|TranslucencyHelper is nullptr"))
		return;
	}
	if (!SpriteComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLWorldItem::ABLWorldItem|SpriteComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Attachments
	TranslucencyHelper->SetupAttachment(RootComponent);
	SpriteComponent->SetupAttachment(TranslucencyHelper);

	// Sprite not allowed to rotate
	SpriteComponent->SetUsingAbsoluteRotation(true);
	// Use TranslucencyHelper parent bound to take care of translucency bounds
	SpriteComponent->bUseAttachParentBound = true;

	// Set collision presets
	TranslucencyHelper->SetCollisionProfileName("NoCollision");
	SpriteComponent->SetCollisionProfileName("NoCollision");

	// Visibility
	TranslucencyHelper->SetVisibility(false);

	// Rotate Roll in -90 so horizontal is X and vertical is Y
	constexpr float RelativeRotationRoll{-90.f};
	SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, RelativeRotationRoll));
}
