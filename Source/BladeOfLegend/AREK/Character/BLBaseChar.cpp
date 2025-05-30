// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLBaseChar.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABLBaseChar::ABLBaseChar()
{
	// Get CapsuleComponent
	Capsule = GetCapsuleComponent();
	// Create SphereComponent helpers for translucency
	TranslucencyHelper = CreateDefaultSubobject<USphereComponent>(TEXT("TranslucencyHelper"));
	TranslucencyHelperBase = CreateDefaultSubobject<USphereComponent>(TEXT("TranslucencyHelperBase"));
	// Get PaperFlipbookComponent
	PaperFlipbook = GetSprite();
	// Create PaperFlipbookComponent for CrystalFlipbook
	CrystalFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CrystalFlipbook"));
	// Get MovementComponent
	Movement = Cast<UCharacterMovementComponent>(ACharacter::GetMovementComponent());

#pragma region NullChecks
	if (!Capsule)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLBaseChar::ABLBaseChar|Capsule is nullptr"))
		return;
	}
	if (!TranslucencyHelper)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLBaseChar::ABLBaseChar|TranslucencyHelper is nullptr"))
		return;
	}
	if (!TranslucencyHelperBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLBaseChar::ABLBaseChar|TranslucencyHelperBase is nullptr"))
		return;
	}
	if (!PaperFlipbook)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLBaseChar::ABLBaseChar|PaperFlipbook is nullptr"))
		return;
	}
	if (!CrystalFlipbook)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLBaseChar::ABLBaseChar|CrystalFlipbook is nullptr"))
		return;
	}
	if (!Movement)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLBaseChar::ABLBaseChar|Movement is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Set collision radius based on sprite extent with pixel per unit
	// This time sprite is 18px width and pixel per unit is 0.25 (now 1)
	// Radius = half sprite width * (1 / pixel per unit) = 9 * (1 / 0.25) = 9 * 4 = 36 (now 9 * (1 / 1) = 9)
	// HalfHeight capsule is 92.f because PlayerStart capsule half height is also 92.f
	Capsule->SetCollisionProfileName(TEXT("PawnIgnorePawn"));
	constexpr float CapsuleRadius{9.f};
	constexpr float CapsuleHalfHeight{92.f};
	Capsule->SetCapsuleRadius(CapsuleRadius);
	Capsule->SetCapsuleHalfHeight(CapsuleHalfHeight);

	// TranslucencyHelpers settings
	// Set sizes of TranslucencyHelpers
	// TranslucencyHelper has to cover whole sprite
	TranslucencyHelper->SetSphereRadius(44.f);
	// BigSphere has to be size of Capsule
	TranslucencyHelperBase->SetSphereRadius(CapsuleRadius);
	// Setup attachments so Sprite (PaperFlipbook) is attached to TranslucencyHelper,
	// TranslucencyHelper is attached to TranslucencyHelperBase,
	// TranslucencyHelperBase is attached to RootComponent (Capsule)
	TranslucencyHelperBase->SetupAttachment(RootComponent);
	TranslucencyHelper->SetupAttachment(TranslucencyHelperBase);
	PaperFlipbook->SetupAttachment(TranslucencyHelper);
	// BigSphere can't rotate
	TranslucencyHelperBase->SetUsingAbsoluteRotation(true);
	// Use TranslucencyHelper parent bound to take care of translucency bounds
	PaperFlipbook->bUseAttachParentBound = true;

	// Set TranslucencyHelper to bottom of Capsule
	TranslucencyHelper->SetRelativeLocation(FVector(0.f, CapsuleRadius, 0.f));
	// TranslucencyHelper can't generate overlap events
	TranslucencyHelper->SetGenerateOverlapEvents(false);
	// Set translucency helpers to not visible in editor
	TranslucencyHelper->SetVisibility(false);
	TranslucencyHelperBase->SetVisibility(false);

	// Move in Y so CapsuleComponent bottom is aligned with feet, and move in Z so sprite is lying on the ground
	constexpr float RelativeLocationY{-9.f};
	constexpr float RelativeLocationZ{-92.f};
	PaperFlipbook->SetRelativeLocation(FVector(0.f, RelativeLocationY, RelativeLocationZ));
	// Rotate Roll in -90 so horizontal is X and vertical is Y
	constexpr float RelativeRotationRoll{-90.f};
	PaperFlipbook->SetRelativeRotation(FRotator(0.f, 0.f, RelativeRotationRoll));
	// Set NoCollision for PaperFlipbook
	PaperFlipbook->SetCollisionProfileName("NoCollision");
	// PaperFlipbook can't rotate
	PaperFlipbook->SetUsingAbsoluteRotation(true);

	// Crystal can't rotate neither
	CrystalFlipbook->SetupAttachment(PaperFlipbook);
	constexpr float RelativeCrystalLocationX{16.f};
	constexpr float RelativeCrystalLocationZ{30.f};
	CrystalFlipbook->SetRelativeLocation(FVector(RelativeCrystalLocationX, 0.f, RelativeCrystalLocationZ));
	// Use PaperFlipbook parent bound to take care of translucency bounds
	CrystalFlipbook->bUseAttachParentBound = true;

	// Set speed
	Movement->MaxWalkSpeed = 150.f;
}
