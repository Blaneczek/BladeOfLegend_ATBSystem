// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLNPCInteract.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"

ABLNPCInteract::ABLNPCInteract()
{
	NameTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	NameTextComponent->SetupAttachment(Cast<USceneComponent>(PaperFlipbook));
	NameTextComponent->SetRelativeLocation(FVector(0.f, 0.f, 44.f));
	NameTextComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	NameTextComponent->SetHorizontalAlignment(EHTA_Center);
	// At start it is hidden because we want to see it only when NPC is in InteractRadius of Player
	NameTextComponent->SetVisibility(false);

	InteractKeyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	InteractKeyWidgetComponent->SetupAttachment(Cast<USceneComponent>(PaperFlipbook));
	InteractKeyWidgetComponent->SetRelativeLocation(FVector(15.f, 0.f, 40.f));
	InteractKeyWidgetComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	InteractKeyWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractKeyWidgetComponent->SetDrawSize(FVector2D(32.f, 32.f));
	// At start it is hidden because we want to see it only when NPC is InteractableActor
	InteractKeyWidgetComponent->SetVisibility(false);
}

void ABLNPCInteract::BeginPlay()
{
	Super::BeginPlay();

	SetTextComponentWithCharacterName();
}

void ABLNPCInteract::SetTextComponentWithCharacterName() const
{
#pragma region NullChecks
	if (!NameTextComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPCBase::SetTextComponentWithCharacterName|TextComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	NameTextComponent->SetText(CharacterName);
}

void ABLNPCInteract::InteractionStart_Implementation(AActor* OtherInstigator)
{
#pragma region NullChecks
	if (!OtherInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPCBase::InteractionStart_Implementation|OtherInstigator is nullptr"))
		return;
	}
#pragma endregion NullChecks

	InteractInstigator = OtherInstigator;
}

void ABLNPCInteract::SetInteractableState_Implementation(bool bCanShowInteract)
{
#pragma region NullChecks
	if (!InteractKeyWidgetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInteractableState_Implementation|InteractKeyWidgetComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	InteractKeyWidgetComponent->SetVisibility(bCanShowInteract);
}

void ABLNPCInteract::InteractionEnd_Implementation()
{
#pragma region NullChecks
	if (!InteractInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPCBase::InteractionEnd_Implementation|InteractInstigator is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const bool bImplementsInterface = InteractInstigator->Implements<UInteractionInterface>();
	if (bImplementsInterface)
	{
		Execute_InteractionEnd(InteractInstigator);
	}
}

AActor* ABLNPCInteract::GetInteractionInstigator_Implementation()
{
#pragma region NullChecks
	if (!InteractInstigator)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ANPCBase::GetInteractionInstigator_Implementation|InteractInstigator is nullptr"))
		return nullptr;
	}
#pragma endregion NullChecks

	return InteractInstigator;
}

void ABLNPCInteract::SetIsInInteractSphere_Implementation(bool bIsInInteractSphere)
{
#pragma region NullChecks
	if (!NameTextComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPCBase::SetIsInInteractSphere_Implementation|TextComponent is nullptr"));
	}
#pragma endregion NullChecks

	NameTextComponent->SetVisibility(bIsInInteractSphere);
}
