// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatPlayerController.h"
#include "BLCombatCamera.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

void ABLCombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (CameraClass)
	{
		if (AActor* Camera = UGameplayStatics::GetActorOfClass(GetWorld(), CameraClass))
		{
			SetViewTarget(Camera);
		}
	}	

	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	SetInputMode(FInputModeGameAndUI());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(MappingContext, 0);
	}

}

void ABLCombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MouseLeftClickAction, ETriggerEvent::Triggered, this, &ABLCombatPlayerController::MouseLeftClick);
		EnhancedInputComponent->BindAction(MouseRightClickAction, ETriggerEvent::Triggered, this, &ABLCombatPlayerController::MouseRightClick);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ABLCombatPlayerController::PauseCombat);
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &ABLCombatPlayerController::SpeedUpGame, true);
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Completed, this, &ABLCombatPlayerController::SpeedUpGame, false);
	}
}

void ABLCombatPlayerController::MouseRightClick()
{
	OnSlotRemoved.ExecuteIfBound();
}

void ABLCombatPlayerController::PauseCombat()
{	
	if (!PauseWidgetClass)
	{
		return;
	}

	if (UUserWidget* PauseWidget = CreateWidget(this, PauseWidgetClass))
	{
		UGameplayStatics::SetGamePaused(GetWorld(),true);
		PauseWidget->AddToViewport();
	}
}

void ABLCombatPlayerController::SpeedUpGame(bool bSpeedUp)
{
	bSpeedUp ? UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 3.f) : UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void ABLCombatPlayerController::MouseLeftClick()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, HitResult))
	{
		OnSlotClicked.ExecuteIfBound(HitResult.GetActor());
	}
}
