// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionComponent.h"
#include "Characters/BLRangeProjectile.h"
#include "Navigation/PathFollowingComponent.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLItemEntryData.h"
#include "BladeOfLegend/DAWID/Items/BLCombatItem.h"
#include "Core/BLCombatSlot.h"
#include "Characters/BLCombatCharacter.h"

// Sets default values for this component's properties
UBLActionComponent::UBLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SlotLocation = FVector(0.f,0.f,0.f);
	AIC = nullptr;
	CurrentAction = nullptr;
	ProjectileTargetsNum = 0;
	ProjectileTargetIndex = 0;
}


// Called when the game starts
void UBLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner()))
	{
		AIC = Cast<AAIController>(OwnerChar->GetController());
	}
}

void UBLActionComponent::CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManagerActor)
{
	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (!OwnerChar)
	{
		EndAction();
		return;
	}

	SlotLocation = OwnerSlotLocation;
	TargetSlots = Targets;

	switch (ActionData.Type)
	{
		case ECombatActionType::ATTACK:
		{
			if (!AttackActions.IsValidIndex(ActionData.Index))
			{
				break;
			}

			CurrentAction = NewObject<UBLAction>(this, AttackActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::DEFEND:
		{
			CurrentAction = NewObject<UBLAction>(this, DefendAction.LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		{
			if (ActionData.CrystalColor != ECrystalColor::NONE && !CrystalActions.Find(ActionData.CrystalColor)->Skills.IsValidIndex(ActionData.Index))
			{
				break;
			}

			CurrentAction = NewObject<UBLAction>(this, CrystalActions.Find(ActionData.CrystalColor)->Skills[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::SPECIAL_SKILL:
		{
			if (!SpecialActions.IsValidIndex(ActionData.Index))
			{
				break;
			}

			CurrentAction = NewObject<UBLAction>(this, SpecialActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::ITEM:
		{
			if (!ItemActions.IsValidIndex(ActionData.Index))
			{
				break;
			}

			// Deleting used item
			if (UBLItemEntryData* ItemEntry = Cast<UBLItemEntryData>(ActionData.ActionEntry))
			{
				ItemEntry->OnDeleteFromList.ExecuteIfBound(ItemEntry->Index);
			}
			else
			{
				break;
			}

			UBLCombatItem* Item = Cast<UBLCombatItem>(ItemActions[ActionData.Index].LoadSynchronous()->GetDefaultObject());
			if (!Item) break;

			CurrentAction = NewObject<UBLAction>(this, Item->Action);
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::RUN_AWAY:
		{
			// 30% chance for escape
			const int32 Random = FMath::RandRange(1, 100);
			if (Random <= 30)
			{
				OnEscapeAction.ExecuteIfBound(true);
			}
			else
			{
				OnEscapeAction.ExecuteIfBound(false);
				EndAction();
			}
			return;
		}
		default: EndAction();
	}
}

void UBLActionComponent::DefaultAction()
{
	if (IsValid(CurrentAction))
	{
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(); });
		CurrentAction->ExecuteAction(nullptr);
	}
	else
	{
		EndAction();
	}
}

void UBLActionComponent::DefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetSlots.IsValidIndex(0) || !IsValid(TargetSlots[0]))
	{
		EndAction();
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination);
	const FVector& Location = TargetSlots[0]->GetCharacter()->GetActorLocation() + (TargetSlots[0]->GetCharacter()->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void UBLActionComponent::DefaultRangeAction(const TSubclassOf<ABLRangeProjectile>& ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetsNum = 0;
	if (!ProjectileClass || !ProjectileSprite || !TargetSlots.IsValidIndex(0) || !IsValid(TargetSlots[0]))
	{
		EndAction();
		return;
	}
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector& Location = GetOwner()->GetActorLocation() + FVector(0.f, -30.f, 0.f);
	const FRotator& Rotation = GetOwner()->GetActorRotation();
		
	if (ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, Location, Rotation, SpawnInfo))
	{
		Projectile->SetData(ProjectileSprite);
		Projectile->OnReachedDestination.BindUObject(this, &UBLActionComponent::ReachedActionDestination);
		Projectile->FlyToTarget(TargetSlots[0]->GetCharacter());
		++ProjectileTargetsNum;
	}
	else
	{
		EndAction();
	}
}

void UBLActionComponent::MultipleDefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetSlots.IsValidIndex(0) || !IsValid(TargetSlots[0]))
	{
		EndAction();
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination, 0);
	const FVector& Location = TargetSlots[0]->GetCharacter()->GetActorLocation() + (TargetSlots[0]->GetCharacter()->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void UBLActionComponent::MultipleDefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetsNum = 0;
	if (ProjectileClass && ProjectileSprite)
	{
		ProjectileTargetIndex = 0;
		ProjectileTargetsNum = TargetSlots.Num();
		FTimerDelegate SpawnDel;
		SpawnDel.BindUObject(this, &UBLActionComponent::SpawnProjectile, ProjectileClass, ProjectileSprite);
		GetWorld()->GetTimerManager().SetTimer(ProjectileSpawnTimer, SpawnDel, 0.3f, true);
	}
	else
	{
		EndAction();
	}
}

void UBLActionComponent::ColumnMeleeAction()
{
	if (!IsValid(AIC) || !TargetSlots.IsValidIndex(0) || !IsValid(TargetSlots[0]))
	{
		EndAction();
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination);
	const FVector& Location = TargetSlots[0]->GetCharacter()->GetActorLocation() + (TargetSlots[0]->GetCharacter()->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void UBLActionComponent::BounceRangeAction(const TSubclassOf<ABLRangeProjectile>& ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetIndex = 1;
	if (!ProjectileClass || !ProjectileSprite || !TargetSlots.IsValidIndex(0) || !IsValid(TargetSlots[0]))
	{
		EndAction();
		return;
	}
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector& Location = GetOwner()->GetActorLocation() + FVector(0.f, -30.f, 0.f);
	const FRotator& Rotation = GetOwner()->GetActorRotation();

	ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, Location, Rotation, SpawnInfo);
	if (!Projectile)
	{
		EndAction();
		return;
	}
	
	Projectile->SetData(ProjectileSprite);
	if (TargetSlots.IsValidIndex(ProjectileTargetIndex))
	{
		Projectile->OnReachedDestination.BindWeakLambda(this, [this, Projectile]()
		{
			ReachedActionDestination(Projectile, 0);
		});
		Projectile->FlyToTargetBounce(TargetSlots[0]->GetCharacter());
	}
	else
	{
		Projectile->OnReachedDestination.BindUObject(this, &UBLActionComponent::ReachedActionDestination);
		Projectile->FlyToTarget(TargetSlots[0]->GetCharacter());
	}
}

void UBLActionComponent::MultipleInPlaceAction(const TSubclassOf<APaperZDCharacter>& EffectClass)
{
	if (!IsValid(CurrentAction))
	{
		EndAction();
		return;
	}
	
	for (const auto& Slot : TargetSlots)
	{
		if (EffectClass && Slot)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APaperZDCharacter>(EffectClass, Slot->GetActorTransform(), SpawnInfo);
		}
	}

	CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(); });
	CurrentAction->ExecuteAction(TargetSlots);
}

// For DefaultMelee and ColumnMelee action flow.
void UBLActionComponent::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		AIC->MoveToLocation(SlotLocation, 5.f);
		EndAction();
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedSlotLocation);

	CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { AIC->MoveToLocation(SlotLocation, 5.f); });
	CurrentAction->ExecuteAction(TargetSlots);
}

// For MultipleMelee action flow.
void UBLActionComponent::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		AIC->MoveToLocation(SlotLocation, 5.f);
		EndAction();
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

	// If there is another target call this function again after reaching location.
	if (TargetSlots.IsValidIndex(TargetIndex + 1) && TargetSlots[TargetIndex + 1] && !TargetSlots[TargetIndex + 1]->GetCharacter()->IsDead())
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination, TargetIndex + 1);
		const FVector& Location = TargetSlots[TargetIndex + 1]->GetCharacter()->GetActorLocation() + (TargetSlots[TargetIndex + 1]->GetCharacter()->GetActorForwardVector() * 50);
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this, Location]() { AIC->MoveToLocation(Location, 5.f); });
	}
	// If it was last target, return do slot.
	else
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedSlotLocation);
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { AIC->MoveToLocation(SlotLocation, 5.f); });
	}

	CurrentAction->ExecuteAction(TargetSlots[TargetIndex]);
}

// For DefaultRange action flow.
void UBLActionComponent::ReachedActionDestination()
{
	if (!IsValid(CurrentAction) || !TargetSlots.IsValidIndex(0) || !IsValid(TargetSlots[0]))
	{
		EndAction();
		return;
	}

	CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(); });
	CurrentAction->ExecuteAction(TargetSlots[0]);
}

// For MultipleRange action flow.
void UBLActionComponent::ReachedActionDestination(int32 Index, bool bLastProjectile)
{
	if (!IsValid(CurrentAction) || !TargetSlots.IsValidIndex(Index) || !IsValid(TargetSlots[Index]))
	{
		EndAction();
		return;
	}

	if (bLastProjectile)
	{
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]()
		{
			EndAction();
		});
	}

	CurrentAction->ExecuteAction(TargetSlots[Index]);
}

// For BounceRange action flow.
void UBLActionComponent::ReachedActionDestination(ABLRangeProjectile* Projectile, int32 Index)
{
	if (!IsValid(CurrentAction) || !TargetSlots.IsValidIndex(Index) || !TargetSlots[Index] || !IsValid(Projectile))
	{
		EndAction();
		return;
	}

	if (!TargetSlots[Index]->GetCharacter()->IsDead())
	{
		CurrentAction->ExecuteAction(TargetSlots[Index]);
	}

	if (TargetSlots.IsValidIndex(Index + 1) && IsValid(TargetSlots[Index + 1]))
	{
		Projectile->OnReachedDestination.BindWeakLambda(this, [this, Index, Projectile]()
		{
			ReachedActionDestination(Projectile, Index + 1);
		});
		Projectile->FlyToTargetBounce(TargetSlots[Index + 1]->GetCharacter());
	}
	else
	{
		Projectile->Destroy();
		EndAction();
	}
}

void UBLActionComponent::ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIC))
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}
	EndAction();
}

void UBLActionComponent::SpawnProjectile(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	if (!TargetSlots.IsValidIndex(ProjectileTargetIndex) || !ProjectileClass)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(ProjectileSpawnTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(ProjectileSpawnTimer);
		}
		EndAction();
		return;
	}
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnInfo))
	{
		bool bLast = TargetSlots.Num() - 1 == ProjectileTargetIndex ? true : false;
		Projectile->SetData(ProjectileSprite);
		Projectile->OnReachedDestination.BindUObject(this, &UBLActionComponent::ReachedActionDestination, ProjectileTargetIndex, bLast);
		Projectile->FlyToTarget(TargetSlots[ProjectileTargetIndex]->GetCharacter());
	}
	
	ProjectileTargetIndex++;
}

void UBLActionComponent::EndAction()
{
	TargetSlots.Empty();

	if (IsValid(CurrentAction))
	{
		CurrentAction->ConditionalBeginDestroy();
		CurrentAction = nullptr;
	}
	OnActionFinished.ExecuteIfBound();
}

void UBLActionComponent::SetActions(const FCombatActions& InCombatActions)
{
	AttackActions = InCombatActions.AttackActions;
	DefendAction = InCombatActions.DefendAction;
	SpecialActions = InCombatActions.SpecialActions;
	CrystalActions = InCombatActions.CrystalActions;
	ItemActions = InCombatActions.ItemActions;
	RunAwayAction = InCombatActions.RunAwayAction;
}

void UBLActionComponent::SetActions(const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	AttackActions = InActions;
}

