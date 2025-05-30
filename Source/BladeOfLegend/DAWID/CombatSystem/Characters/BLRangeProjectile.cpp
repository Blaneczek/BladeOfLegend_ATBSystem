// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLRangeProjectile.h"
#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BLCombatCharacter.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
ABLRangeProjectile::ABLRangeProjectile()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Movement->MaxWalkSpeed = 500.f;
	PaperFlipbook->SetUsingAbsoluteRotation(false);
	TranslucencyHelperBase->SetUsingAbsoluteRotation(false);
}

// Called when the game starts or when spawned
void ABLRangeProjectile::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());
}

void ABLRangeProjectile::SetData(UPaperFlipbook* ProjectileSprite)
{
	if (IsValid(PaperFlipbook))
	{
		PaperFlipbook->SetFlipbook(ProjectileSprite);
	}
}

void ABLRangeProjectile::FlyToTarget(ABLCombatCharacter* Target)
{
	if (IsValid(AIController) && Target)
	{	
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLRangeProjectile::ReachedDestination);
		const FVector& Location = Target->GetActorLocation() + FVector(0.f, -30.f, 0.f);
		AIController->MoveToLocation(Location, 5.f);
	}
}

void ABLRangeProjectile::FlyToTargetBounce(ABLCombatCharacter* Target)
{
	if (IsValid(AIController) && Target)
	{
		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLRangeProjectile::ReachedBounceDestination);	
		const FVector& Location = Target->GetActorLocation() + FVector(0.f, -30.f, 0.f);
		AIController->MoveToLocation(Location, 5.f);
	}
}

void ABLRangeProjectile::ReachedDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIController))
	{
		AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		OnReachedDestination.ExecuteIfBound();
		Destroy();
	}
}

void ABLRangeProjectile::ReachedBounceDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIController))
	{
		AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		OnReachedDestination.ExecuteIfBound();
	}
}

