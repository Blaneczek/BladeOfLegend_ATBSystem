// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PaperFlipbookComponent.h"
#include "BladeOfLegend/AREK/Camera/BLCameraDefault.h"
#include "BladeOfLegend/AREK/Camera/BLSpringArmDefault.h"
#include "BladeOfLegend/AREK/Character/Companion/BLCompanion.h"
#include "BladeOfLegend/AREK/Character/Companion/AI/BLCompanionAIC.h"
#include "BladeOfLegend/AREK/Interfaces/Interaction/InteractionInterface.h"
#include "BladeOfLegend/AREK/GameInstance/BLGameInstance.h"
#include "Components/SphereComponent.h"

ABLPlayer::ABLPlayer()
{
	// Setup InteractSphere
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	InteractSphere->SetupAttachment(RootComponent);
	InteractSphere->SetSphereRadius(InteractSphereRadius);
	InteractSphere->SetUsingAbsoluteRotation(true);

	// Setup SpringArm
	SpringArm = CreateDefaultSubobject<UBLSpringArmDefault>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteLocation(true);
	SpringArm->SetUsingAbsoluteRotation(true);
	const FRotator NewRotation{-90.f, 0.f, -90.f};
	SpringArm->SetRelativeRotation(NewRotation);
	SpringArm->TargetArmLength = 700.f;

	// Setup Camera
	Camera = CreateDefaultSubobject<UBLCameraDefault>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ABLPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayerController();
	SetupInputMappingContext();
	BindInteractSphereOverlapFunctions();

	UpdateFlipbookFromDAHeroes();

	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	CrystalFlipbook->SetFlipbook(
		GI->GetSaveGameData().HeroesData.Heroes[0].CrystalsData[GI->GetSaveGameData().HeroesData.Heroes[0].
			CurrentCrystal].CrystalFlipbook);
}

void ABLPlayer::InitializePlayerController()
{
	PlayerController = Cast<APlayerController>(GetController());
}

void ABLPlayer::UpdateFlipbookFromDAHeroes() const
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());

#pragma region NullChecks
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::UpdateFlipbookFromDAHeroes|GI is nullptr"))
		return;
	}

	if (GI->GetSaveGameData().HeroesData.Heroes.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::UpdateFromDAHeroes|HeroesData.Heroes.Num() <= 0"))
		return;
	}
#pragma endregion NullChecks

	FHeroAssetInfo HeroAssetInfo = GI->GetSaveGameData().HeroesData.Heroes[0];

#pragma region NullChecks
	if (!HeroAssetInfo.Sprite)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::UpdateFromDAHeroes|Sprite is nullptr"))
		return;
	}
	//if (!HeroAssetInfo.OutOfCombatAnimInstanceClass)
	//{
	//UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::UpdateFromDAHeroes|OutOfCombatAnimInstanceClass is nullptr"))
	//return;
	//}
#pragma endregion NullChecks


#pragma region NullChecks
	if (!HeroAssetInfo.CrystalsData.Contains(HeroAssetInfo.CurrentCrystal))
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateFlipbookFromDAHeroes|CrystalsMap doesn't contain CurrentCrystal"))
		return;
	}
	if (!HeroAssetInfo.CrystalsData[HeroAssetInfo.CurrentCrystal].CrystalFlipbook)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateFlipbookFromDAHeroes|CrystalFlipbook in DA is nullptr"))
		return;
	}
#pragma endregion NullChecks

	CrystalFlipbook->SetFlipbook(HeroAssetInfo.CrystalsData[HeroAssetInfo.CurrentCrystal].CrystalFlipbook);
}

void ABLPlayer::AddCompanions()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());

#pragma region NullChecks
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::AddCompanions|GI is nullptr"))
		return;
	}

	if (!CompanionClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::AddCompanions|CompanionClass is nullptr"))
		return;
	}
#pragma endregion NullChecks

	const TArray<FHeroAssetInfo> HeroesAssetInfo = GI->GetSaveGameData().HeroesData.Heroes;
	// Max 4 companions
	const uint8 CompanionsNum = FMath::Clamp(HeroesAssetInfo.Num(), 1, 5);

	// Iterate through HeroesAssetInfo, but start at 1 (because 0 is player) and end at 5 (because max 4 companions)
	for (uint8 i = 1; i < CompanionsNum; i++)
	{
		FHeroAssetInfo CompanionInfo = HeroesAssetInfo[i];
		ABLBaseChar* FollowTarget;

		// If adding not first companion, follow last companion
		if (ActiveCompanions.Num() > 0)
		{
			FollowTarget = ActiveCompanions.Last();
		}
		// If adding first companion, follow player
		else
		{
			FollowTarget = this;
		}

		// Set transform of companion to spawn
		FTransform FollowTargetTransform{FollowTarget->GetActorTransform()};
		FVector FollowTargetForwardVector{FollowTarget->GetActorForwardVector()};

		// Negate LineSpacing so it will spawn behind
		FVector CompanionLocation{FollowTargetTransform.GetLocation() + FollowTargetForwardVector * -LineSpacing};
		FRotator CompanionRotation{FollowTargetTransform.GetRotation()};

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABLCompanion* SpawnedCompanion = GetWorld()->SpawnActor<ABLCompanion>(
			CompanionClass, CompanionLocation, CompanionRotation, SpawnInfo);

#pragma region NullChecks
		if (!SpawnedCompanion)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::AddCompanions|SpawnedCompanion is nullptr"))
			return;
		}
#pragma endregion NullChecks

		// Add spawned companion to active companions
		ActiveCompanions.Add(SpawnedCompanion);
		// Get controller and set follow target of spawned companion
		ABLCompanionAIC* SpawnedCompanionController = Cast<ABLCompanionAIC>(SpawnedCompanion->GetController());

#pragma region NullChecks
		if (!SpawnedCompanionController)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::AddCompanions|SpawnedCompanionController is nullptr"))
			return;
		}
#pragma endregion NullChecks

		SpawnedCompanionController->SetFollowTarget(FollowTarget);

		// Set Flipbook and AnimInstance of spawned companion
		//SpawnedCompanion->GetSprite()->SetFlipbook(CompanionInfo.Sprite);
		//SpawnedCompanion->GetAnimationComponent()->SetAnimInstanceClass(CompanionInfo.OutOfCombatAnimInstanceClass);
	}
}

void ABLPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

#pragma region NullChecks
	if (!IA_Interact)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::SetupPlayerInputComponent|IA_Interact is nullptr"))
		return;
	}
	if (!IA_Move)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::SetupPlayerInputComponent|IA_Move is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Return if cast failed
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

#pragma region NullChecks
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::SetupPlayerInputComponent|EnhancedInputComponent is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Bind OnInteractionStart method with IA_Interact
	EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &ABLPlayer::OnInteract);
	// Bind OnMove method with IA_Move
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABLPlayer::OnMove);
}

void ABLPlayer::SetupInputMappingContext() const
{
#pragma region NullChecks
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::SetupInput|PlayerController is nullptr"))
		return;
	}
	if (!IMC_Default)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::SetupInput|MappingContext is nullptr"))
		return;
	}
#pragma endregion NullChecks

	// Get InputSubsystem
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>();

	// Set MappingContext
	InputSubsystem->AddMappingContext(IMC_Default, 0);
}

void ABLPlayer::OnInteract()
{
	// Return if no InteractableActor found
	if (!InteractableActor)
	{
		return;
	}

	// If InteractableActor has InteractionInterface fire InteractionStart
	const bool bImplementsInterface = InteractableActor->Implements<UInteractionInterface>();
	if (bImplementsInterface)
	{
		IInteractionInterface::Execute_InteractionStart(InteractableActor, this);
	}
}

void ABLPlayer::OnInteractSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	const bool bImplementsInterface = OtherActor->Implements<UInteractionInterface>();
	if (!bImplementsInterface || OtherActor == this || ActorsInInteractSphereArray.Contains(OtherActor))
	{
		return;
	}

	ActorsInInteractSphereArray.Add(OtherActor);
	IInteractionInterface::Execute_SetIsInInteractSphere(OtherActor, true);

	// Call this function because OnMove is not enough
	ChooseInteractableActor();
}

void ABLPlayer::OnInteractSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const bool bImplementsInterface = OtherActor->Implements<UInteractionInterface>();
	if (!bImplementsInterface || OtherActor == this || !ActorsInInteractSphereArray.Contains(OtherActor))
	{
		return;
	}

	ActorsInInteractSphereArray.Remove(OtherActor);
	IInteractionInterface::Execute_SetIsInInteractSphere(OtherActor, false);

	// Call this function because OnMove is not enough
	ChooseInteractableActor();
}

void ABLPlayer::BindInteractSphereOverlapFunctions()
{
#pragma region NullChecks
	if (!InteractSphere)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLPlayer::BindInteractSphereOverlapFunctions|IntractRadius is nullptr"))
		return;
	}
#pragma endregion NullChecks

	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ABLPlayer::OnInteractSphereBeginOverlap);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ABLPlayer::OnInteractSphereEndOverlap);
}

void ABLPlayer::SetInteractableActor(TObjectPtr<AActor> Actor)
{
	// Disable the current interactable actor if it exists
	if (InteractableActor)
	{
		const bool bImplementsInterface = InteractableActor->Implements<UInteractionInterface>();
		if (bImplementsInterface)
		{
			IInteractionInterface::Execute_SetInteractableState(InteractableActor, false);
		}
	}

	// Enable the new actor if it's not null
	if (Actor)
	{
		const bool bImplementsInterface = Actor->Implements<UInteractionInterface>();
		if (bImplementsInterface)
		{
			IInteractionInterface::Execute_SetInteractableState(Actor, true);
		}
	}

	InteractableActor = Actor;
}

void ABLPlayer::ChooseInteractableActor()
{
	// If no actors in interact sphere, interactable actor is none, and return function
	if (ActorsInInteractSphereArray.IsEmpty())
	{
		SetInteractableActor(nullptr);
		return;
	}
	// If only one actor in interact sphere, set him as interactable actor and return function
	if (ActorsInInteractSphereArray.Num() == 1)
	{
		AActor* Actor = ActorsInInteractSphereArray[0];
		SetInteractableActor(Actor);
		return;
	}

	constexpr float Offset{100.f};
	float ClosestDistanceToPlayer{InteractSphereRadius + Offset};
	AActor* ClosestActorInInteractSphere{nullptr};
	const FVector PlayerLocation = GetActorLocation();

	// Iterate through actors in interact sphere and choose the closest one
	for (int i = 0; i < ActorsInInteractSphereArray.Num(); i++)
	{
		AActor* ActorInInteractSphere = ActorsInInteractSphereArray[i];
		FVector InteractableActorLocation = ActorInInteractSphere->GetActorLocation();

		const float DistanceToPlayer = FVector::Dist2D(PlayerLocation, InteractableActorLocation);

		if (DistanceToPlayer < ClosestDistanceToPlayer)
		{
			ClosestDistanceToPlayer = DistanceToPlayer;
			ClosestActorInInteractSphere = ActorInInteractSphere;
		}
	}

	SetInteractableActor(ClosestActorInInteractSphere);
}

void ABLPlayer::OnMove(const FInputActionValue& Value)
{
	const float ActionValueX = Value.Get<FVector2D>().X;
	const float ActionValueY = -Value.Get<FVector2D>().Y; // Y is negative because map is pos X and neg Y oriented

	MoveInDirection(ActionValueX, ActionValueY);
	RotateInDirection(ActionValueX, ActionValueY);

	ChooseInteractableActor();
}

void ABLPlayer::MoveInDirection(const float X, const float Y)
{
	const FVector MoveDirection = FVector(X, Y, 0.f);
	AddMovementInput(MoveDirection);
}

void ABLPlayer::RotateInDirection(const float X, const float Y)
{
	const float AngleRadians = FMath::Atan2(Y, X);
	const float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	const FRotator NewActorRotation = FRotator(0.f, AngleDegrees, 0.f);
	SetActorRotation(NewActorRotation);
}
