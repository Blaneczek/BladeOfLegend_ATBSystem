<sub>Unreal Engine version 5.5</sub>
</br>

# Active Time Battle System
Active Time Battle is a type of turn-based combat system in which each character has its own time bar to attack at its own pace. There is no pause for your action, as in the classic turn-based system.

![atb_system](https://github.com/user-attachments/assets/dd45f40c-6826-458c-b025-c6fe10f13a89)

# Introduction
The system was created while participating in a game jam. The team and I were developing a classic 2D RPG with turn-based combat. The game was made using Unreal Engine 5 and C++. 
</br>My task was to create the combat system and implement the designers' ideas, as well as integrate the system with the game's UI. 
</br>The system consists of 6 types of activities: attack, defense, crystal skills, weapon skills, items, escape. Each of them uses actions which are templates for skills which can be easily created in 9 different prepared types. 
</br>The video demonstrates a sample gameplay showing most of the elements of the system: (https://www.youtube.com/watch?v=getVlf-3iAM or click the image below)

[![GAMEPLAY](https://img.youtube.com/vi/getVlf-3iAM/0.jpg)](https://www.youtube.com/watch?v=getVlf-3iAM)

# Playable demo
Together with the team, we have created a playable demo where you can test the system in action. The demo can be downloaded on the itchi.io platform:
[https://blaneczek.itch.io/blade-of-legend](https://blaneczek.itch.io/blade-of-legend)

# Key elements

|                                                                               | Description                                                     |
|-------------------------------------------------------------------------------|-----------------------------------------------------------------|
| [Actions](#actions-code)                                                      | Templates for abilities.                                        |
| [Combat characters](#combat-characters-code)                                  | Heroes and enemies overview.                                    |
| [Core](#core-code)                                                            | System managers.                                                |
| [UI](#ui-code)                                                                | Integrated widgets with the system.                             |


# Actions ([code](Source/BladeOfLegend/DAWID/Actions))  
<details>
<summary>More</summary>
</br>	
Actions are used for all activities that characters perform during combat, from using abilities to using items. Each action is a UObject that is created when any activity is performed. There are 9 types of actions that determine how an activity will be performed: </br>
  
<img src="https://github.com/user-attachments/assets/b5b246d9-91e8-4e83-b154-97bad793ce8a" width="800">

</br>**Default:** an action that is used on the character who performs it, such as using items or the Defense ability.
</br>![default](https://github.com/user-attachments/assets/484e87a2-7d96-4baa-95b8-bdd5fe2d6283)

</br>**Default Melee/Range:** an action that is used on another character, such as using the default attack ability. In Melee the character runs to the target, in Range the character sends a projectile to the target.
</br>![defaultmelee (2)](https://github.com/user-attachments/assets/0d04fef0-a954-4523-b9d2-9200ec76cbcc) ![defaultrange](https://github.com/user-attachments/assets/164f3493-a458-4238-82b7-2ebbe9076a64)

</br>**Multiple Default Melee/Range:** an action used on multiple characters.
</br>![multiplemelee](https://github.com/user-attachments/assets/09b024bf-3aff-41e6-a9f8-e5e1918bba53) ![multiplerange](https://github.com/user-attachments/assets/c3113ccd-f273-4f6e-bf90-87cfaa36e45a)

</br>**Bounce Range:** a special action that sends a single projectile that bounces between multiple characters.
</br>![bounce](https://github.com/user-attachments/assets/12ae90be-cd0a-471c-9056-d5cf5d9f340e)

</br>**Column Melee:** a special action in which one column of slots is selected and the character performs the ability on every character in that column
</br>![column](https://github.com/user-attachments/assets/c34676cb-2964-4aa4-8fed-5843c80b5c93)

</br>**Whole Team In Place:** a special action in which the ability is used on all enemies without projectiles or having to walk up to them..
</br>![wholeteam](https://github.com/user-attachments/assets/249a9654-e3ab-4c3c-ab6d-9b360492294f)

</br>**Summon:** a special action that allows you to summon new characters to slots.
</br>![summon](https://github.com/user-attachments/assets/096a0d5e-dcf4-4a06-b8e0-a25141f1518f)

</br>Let's take a look at how this works. For example, the player selects **Attack**, selects the enemy character, and then the proper UObject is created. An action of type **DefaultMelee** is used for this purpose. The character's default attack is created in Blueprint, which inherits from the **DefaultMeleeAction** class. This way designers can easily create abilities in Blueprints, set all the necessary data and calculate damage or other effects. </br> 

<img src="https://github.com/user-attachments/assets/5f530f65-0e81-433f-a5ac-ebcfae6f0f37" width="1000">

</br>Actions are managed by **BLActionComponent**, which is added to Characters. In this component, the correct action type is first created in the **CreateAction** function based on **ActionData.Type**. Then the **OnCreateAction** function of the newly created UObject is called.

```c++
void UBLActionComponent::CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManagerActor)
{
	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (!OwnerChar)
	{
		EndAction(true);
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
  .
  .
  .
```

</br>The **ActivateAction** function is then called in the Action object. Each action type has its own implementation of this function. The action we chose earlier is of type **DefaultMeleeAction**. There we return to the ActionComponent and call the **DefaultMeleeAction** function. 

```c++
void UBLAction::OnCreateAction(UBLActionComponent* OwnerComponent, ABLCombatCharacter* Owner, AActor* InCombatManager)
{
	if (OwnerComponent && Owner)
	{
		CombatManager = InCombatManager;
		OwnerChar = Owner;

		ActivateAction(OwnerComponent);

		if (TurnsCost > 0)
		{
			OwnerChar->StartActionTurnsCooldown(TurnsCost);
		}
	}
}
```

```c++
void UBLDefaultMeleeAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (OwnerComponent && IsValid(OwnerChar))
	{
		OwnerComponent->DefaultMeleeAction();

		OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
	}
}
```

</br>**ActionComponent** manages all the action flows. In our example, the **DefaultMeleeAction** function is called, which calculates the location where our character needs to move and sends it there.

```c++
/** Action is executing in place, target is the owner. */
	void DefaultAction();

	/** Character runs up to the target and executes action. */
	void DefaultMeleeAction();

	/** Character creates a projectile that flies to the target and executes action. */
	void DefaultRangeAction(const TSubclassOf<ABLRangeProjectile>& ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character runs up to the targets one by one and executes action for every target .*/
	void MultipleDefaultMeleeAction();

	/** Character creates multiple projectiles that fly to the targets and execute action. */
	void MultipleDefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character runs up to the column and executes an action for every target in column. */
	void ColumnMeleeAction();

	/** Character creates a single projectile that travels through multiple targets. */
	void BounceRangeAction(const TSubclassOf<ABLRangeProjectile>& ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character in place executes an action for each target. */
	void MultipleInPlaceAction(const TSubclassOf<APaperZDCharacter>& EffectClass);
```

```c++
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
```

</br>When a character reaches the target point, an action is executed (an animation is played, damage is dealt, etc.), and then the characters return to their location.

```c++
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
```

```c++
void UBLDefaultMeleeAction::ExecuteAction(const TArray<ABLCombatSlot*>& Targets)
{
	if (!IsValid(OwnerChar) || !Targets.IsValidIndex(0) || !Targets[0])
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (IsValid(ActionAnim))
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		ActionCalculations(Targets[0], CombatManager);
	}
	else
	{
		ActionCalculations(Targets[0], CombatManager);
		OnEndExecution.ExecuteIfBound();
	}
}
```

<br>
</details>

# Combat characters ([code](Source/BladeOfLegend/DAWID/Characters)) 
<details>
<summary>More</summary>
	
</br>**Combat characters** are heroes and enemies used during combat. They contain all the necessary data, such as character attributes and possessed actions, as well as functions responsible for calculating damage and other effects.

```c++
float ABLCombatCharacter::CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal)
{ 
	OutIsHeal = false;

	TArray<TArray<float>> ElementsTable = {
	  // AttackType
	  //   FIRE     WATER   EARTH   WIND    ICE    THUNDER  ACID    DARK    WHITE   NONE       TargetType
		{1.0f,   2.0f,   1.0f,   1.0f,   1.5f,   1.0f,   1.0f,   1.5f,   1.0f,   1.0f},  // FIRE   
		{2.0f,   1.0f,   1.0f,   1.0f,   1.5f,   4.0f,   0.75f,  1.0f,   1.5f,   1.0f},  // WATER
		{1.0f,   1.0f,   1.0f,   2.0f,   1.0f,   0.0f,   4.0f,   1.5f,   1.0f,   1.0f},  // EARTH
		{1.0f,   1.0f,   2.0f,   1.0f,   1.0f,   4.0f,   1.0f,   1.0f,   1.5f,   1.0f},  // WIND
		{1.5f,   0.75f,  1.0f,   1.0f,   1.0f,   0.75f,  1.0f,   1.5f,   1.0f,   1.0f},  // ICE
		{1.0f,   0.0f,   4.0f,   0.0f,   1.5f,   1.0f,   1.0f,   1.0f,   1.5f,   1.0f},  // THUNDER
		{1.0f,   1.5f,   0.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.5f,   1.0f,   1.0f},  // ACID
		{0.0f,   1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   2.0f,   0.0f},  // DARK
		{1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   2.0f,   1.0f,   0.0f},  // WHITE
		{1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f}   // NONE
	};

	const int32 AttackElementIndex = static_cast<int32>(DamageElementType);
	const int32 TargetElementIndex = static_cast<int32>(CharacterElementType);

	const float Multiplier = ElementsTable[TargetElementIndex][AttackElementIndex];

	// If Attack and Target Element is the same, damage will be converted to healing (except NONE NONE)
	if (AttackElementIndex == TargetElementIndex && AttackElementIndex != ElementsTable.Num() - 1)
	{
		OutIsHeal = true;
		return Multiplier;
	}
	else
	{
		return Multiplier;
	}
}
```

```c++
void ABLCombatCharacter::HandleDamageHit(ABLCombatCharacter* Attacker, float Damage, float DMGMultiplier, ECombatElementType DamageElementType, bool bMagicalAction)
{
	// Only if action is physical
	if (!bMagicalAction)
	{
		// If it draws DODGE, character will not take any damage or heal
		const int32 DodgeChance = FMath::RandRange(1, 100);
		if (DodgeChance <= CurrentDodge)
		{
			DisplayTextDMG(0, false, DamageElementType, true);
			return;
		}
	}
	
	if (bMagicalAction && bMagicImmunity)
	{
		DisplayTextDMG(0, false, DamageElementType, false);
		return;
	}

	// If it draws Pierce, Defense is reduced by half
	const int32 PierceChance = FMath::RandRange(1, 100);
	const float NewDefense = PierceChance <= BaseData.Pierce ? CurrentDefense / 2 : CurrentDefense;

	// 10 def decreases dmg by 5%
	float DMGValue = (Damage * DMGMultiplier) * (1.f - ((NewDefense / 1000) * 5));

	// If attack is physical and Attacker has Poisoning status, dmg is decreased by 20%
	if (!bMagicalAction && Attacker && Attacker->StatusesComponent->Statuses.Contains(ECombatStatusType::POISONING))
	{
		// Clamp because Defense can be higher than Damage, so that Damage is not negative
		DMGValue = FMath::Clamp(FMath::RoundHalfFromZero(DMGValue * 0.8f), 0, FMath::RoundHalfFromZero(DMGValue));
	}
	else
	{
		DMGValue = FMath::Clamp(FMath::RoundHalfFromZero(DMGValue), 0, FMath::RoundHalfFromZero(DMGValue));
	}

	// Sets character HP between 0 and MaxHP
	CurrentHP = FMath::Clamp((CurrentHP - DMGValue), 0, BaseData.MaxHP);

	DisplayTextDMG(DMGValue, false, DamageElementType);
	OnHealthUpdate.ExecuteIfBound();

	if (BaseData.TakeDMGAnim && GetAnimInstance() && !bDefendIdle)
	{
		GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.TakeDMGAnim);
	}
}
```


</details>

# Core ([code](Source/BladeOfLegend/DAWID/Core)) 
<details>
<summary>More</summary>
	
</br>There are two key elements: **Combat Slots** and **Combat Manager**. 
</br>
</br>**Combat slots** are actors with collision boxes placed on the level where combat characters are located. Each character is assigned to a slot. The player can interact with the assigned slots using the mouse. When the player hovers over or clicks on a slot, an arrow of a different color appears. Slots are used to select heroes and enemies by the player.
	
<img src="https://github.com/user-attachments/assets/74257afa-30b0-41d6-abfa-187f43de648c" width="415"> ![slotsgif](https://github.com/user-attachments/assets/74bc39c9-28da-4318-87d1-637c773c6a39)

</br>**Combat Manager** is the main part of the system that manages gameplay, and also UI elements. We can break it down into 3 parts (Player, Enemies and General) and describe the most important functions.
</br></br>**Player:** The manager spawns all heroes with the appropriate data and handles clicking on slots when the player performs an action. Then, when the action is used, it processes it further, adding it to the queue.

```c++
void ABLCombatManager::SetPlayerTeam()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());	
	if (!GI || !IsValid(Widget))
	{
		return;
	}

	for (int32 Index = 0; Index < FMath::Clamp(GI->SaveGameData.HeroesData.Heroes.Num(), 0, 5); ++Index)
	{
		const FCombatCharData& CharBaseData = GI->CalculateBaseCombatData(Index);
		if (PlayerTeam[Index])
		{
			PlayerTeam[Index]->SpawnHero(CharBaseData, GI->SaveGameData.HeroesData.Heroes[Index].CombatActions, GI->CombatData.bSneakAttack);
			GI->CombatData.bSneakAttack ? Widget->AddHero(PlayerTeam[Index]->GetIndex(), CharBaseData, true)
										: Widget->AddHero(PlayerTeam[Index]->GetIndex(), CharBaseData, false);
			Widget->AddHeroActions(PlayerTeam[Index]->GetIndex(), CharBaseData, GI->SaveGameData.HeroesData.Heroes[Index].CombatActions, GI->CombatData.bCanRunAway);
		}
	}
}
```

```c++
void ABLCombatManager::HandleSlotClicked(AActor* Slot)
{
	if (CurrentActionType == ECombatActionType::NONE)
	{
		return;
	}
	
	ABLCombatSlot* CurrentSlot = Cast<ABLCombatSlot>(Slot);
	if (!CurrentSlot || !CurrentSlot->IsActive())
	{
		return;
	}

	// If the player doesn't have enough ME for an action, display the widget.
	if (IsValid(Widget) && CurrentActionData.MECost > CurrentPlayerSlot->GetCharacter()->GetCurrentME())
	{
		Widget->ActivateNotEnoughME();
		return;
	}

	switch (CurrentActionData.Flow) 
	{
		case ECombatActionFlow::DEFAULT:
		{
			if (CurrentSlot == CurrentPlayerSlot)
			{
				ChooseTargetSlot(CurrentSlot);
				break;
			}
			return;
		}
		case ECombatActionFlow::DEFAULT_MELEE:
		case ECombatActionFlow::DEFAULT_RANGE:
		case ECombatActionFlow::MULTIPLE_MELEE:
		case ECombatActionFlow::MULTIPLE_RANGE:
		{
			if (CurrentSlot->IsEnemy())
			{
				ChooseTargetSlot(CurrentSlot);
				if (CurrentTargetsSlots.Num() >= CurrentActionData.TargetsNum)
				{
					break;
				}
			}
			return;	
		}
		case ECombatActionFlow::COLUMN_MELEE:
		{
			if (CurrentSlot->IsEnemy())
			{
				int32 IndexStart = 0;
				int32 IndexEnd = 0;
				// 1. column
				if (CurrentSlot->GetIndex() <= 3)
				{
					IndexStart = 0;
					IndexEnd = 3;
				}
				// 2. column
				else if (CurrentSlot->GetIndex() >= 4 && CurrentSlot->GetIndex() <= 7)
				{
					IndexStart = 4;
					IndexEnd = 7;
				}
				// 3. column
				else
				{
					IndexStart = 8;
					IndexEnd = 11;
				}

				for (IndexStart; IndexStart <= IndexEnd; ++IndexStart)
				{
					if (EnemyTeam[IndexStart] && EnemyTeam[IndexStart]->IsActive())
					{
						ChooseTargetSlot(EnemyTeam[IndexStart]);
					}
				}
				break;
			}
			return;				
		}
		case ECombatActionFlow::BOUNCE_RANGE:
		{
			if (CurrentSlot->IsEnemy())
			{
				ChooseTargetSlot(CurrentSlot);
				if (CurrentActionData.TargetsNum == 1)
				{
					break;
				}

				// Projectiles cannot bounce 2 times in a row on the same target
				int32 DisabledIndex = CurrentSlot->GetIndex();
				for (int32 Index = 0; Index < CurrentActionData.TargetsNum - 1; ++Index)
				{
					TArray<int32> AvailableIndexes;
					for (const auto& EnemySlot : EnemyTeam)
					{
						if (EnemySlot && EnemySlot->IsActive() && EnemySlot->GetIndex() != DisabledIndex)
						{
							AvailableIndexes.Add(EnemySlot->GetIndex());
						}
					}

					if (AvailableIndexes.IsEmpty())
					{
						break; 
					}

					const int32 RandomIndex = FMath::RandRange(0, AvailableIndexes.Num() - 1);						
					CurrentTargetsSlots.Add(EnemyTeam[AvailableIndexes[RandomIndex]]);
					DisabledIndex = AvailableIndexes[RandomIndex];
				}
				break;
			}
			return;
		}
		case ECombatActionFlow::WHOLE_TEAM_IN_PLACE:
		{
			if (CurrentSlot->IsEnemy())
			{	
				for (const auto& EnemySlot : EnemyTeam)
				{
					if (EnemySlot && EnemySlot->IsActive())
					{
						ChooseTargetSlot(EnemySlot);
					}
				}
				break;
			}
			return;
		}
		default:
		{
			return;
		}
	}

	ProcessPlayerAction();
}
```

```c++
void ABLCombatManager::ProcessPlayerAction()
{
	if (!IsValid(Widget) || !IsValid(CurrentPlayerSlot))
	{
		return;
	}

	AddActionToQueue(CurrentPlayerSlot, CurrentTargetsSlots, CurrentActionData, false);
	CurrentPlayerSlot->bCanDoAction = false;
	CurrentActionType = ECombatActionType::NONE;

	Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());

	ClearPlayerSlot();
	ChooseAvailablePlayerSlot();

	FTimerHandle ClearTargetDelay;
	FTimerDelegate ClearTargetDel;
	ClearTargetDel.BindUObject(this, &ABLCombatManager::ClearTargetsSlots);
	GetWorld()->GetTimerManager().SetTimer(ClearTargetDelay, ClearTargetDel, 1.f, false);
}
```

</br>**Enemies:** The manager spawns all enemies with the appropriate data and handles their actions.

```c++
oid ABLCombatManager::HandleEnemyAction(ABLCombatSlot* EnemySlot, FCombatActionData&& ActionData)
{
	TArray<int32> ActiveSlots;
	for (const auto& Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			ActiveSlots.Add(Slot->GetIndex());
		}
	}

	if (ActiveSlots.IsEmpty())
	{
		return;
	}

	TArray<ABLCombatSlot*> Targets;

	switch (ActionData.Flow)
	{
		case ECombatActionFlow::DEFAULT:
		{
			Targets.Add(EnemySlot);
			break;
		}
		case ECombatActionFlow::DEFAULT_MELEE:
		case ECombatActionFlow::DEFAULT_RANGE:
		case ECombatActionFlow::MULTIPLE_MELEE:
		case ECombatActionFlow::MULTIPLE_RANGE:
		{
			for (int32 Index = 0; Index < ActionData.TargetsNum; ++Index)
			{
				const int32 RandomIndex = FMath::RandRange(0, ActiveSlots.Num() - 1);
				Targets.Add(PlayerTeam[ActiveSlots[RandomIndex]]);
			}
			break;		
		}
		case ECombatActionFlow::BOUNCE_RANGE:
		{
			int32 RandomIndex = FMath::RandRange(0, ActiveSlots.Num() - 1);
			Targets.Add(PlayerTeam[ActiveSlots[RandomIndex]]);
			int32 DisabledIndex = ActiveSlots[RandomIndex];

			for (int32 Index = 0; Index < ActionData.TargetsNum - 1; ++Index)
			{
				TArray<int32> AvailableIndexes;
				for (const auto& ActiveIndex : ActiveSlots)
				{
					if (ActiveIndex != DisabledIndex)
					{
						AvailableIndexes.Add(ActiveIndex);
					}
				}
				if (AvailableIndexes.IsEmpty())
				{
					break;
				}
				RandomIndex = FMath::RandRange(0, AvailableIndexes.Num() - 1);
				Targets.Add(PlayerTeam[AvailableIndexes[RandomIndex]]);
				DisabledIndex = AvailableIndexes[RandomIndex];
			}
			break;
		}
		case ECombatActionFlow::SUMMON_ALLIES:
		{
			TArray<int32> AvailableSlotsIndex;
			for (const auto& Slot : EnemyTeam)
			{
				// not active because we are summoning in free slots
				if (Slot && !Slot->IsActive() && Slot != EnemySlot)
				{
					AvailableSlotsIndex.Add(Slot->GetIndex());
				}
			}

			if (AvailableSlotsIndex.IsEmpty())
			{
				Targets.Add(FindNewTargetSlot(true));
				ActionData = FCombatActionData(ECombatActionType::ATTACK, ECombatActionFlow::DEFAULT_MELEE, 0);
				break;
			}

			for (int32 Index = 0; Index < ActionData.TargetsNum; ++Index)
			{
				if (AvailableSlotsIndex.IsEmpty())
				{
					break;
				}
				const int32 RandomIndex = FMath::RandRange(0, AvailableSlotsIndex.Num() - 1);
				Targets.Add(EnemyTeam[AvailableSlotsIndex[RandomIndex]]);
				AvailableSlotsIndex.RemoveAt(RandomIndex);
			}
			break;
		}
		case ECombatActionFlow::KILL_ALLIES:
		{
			TArray<int32> AvailableSlotsIndex;
			for (auto& Slot : EnemyTeam)
			{
				if (Slot && Slot->IsActive() && Slot != EnemySlot)
				{
					AvailableSlotsIndex.Add(Slot->GetIndex());
				}
			}

			if (AvailableSlotsIndex.IsEmpty())
			{
				Targets.Add(FindNewTargetSlot(true));
				ActionData = FCombatActionData(ECombatActionType::ATTACK, ECombatActionFlow::DEFAULT_MELEE, 0);
				break;
			}

			const int32 Random = FMath::RandRange(1, ActionData.TargetsNum);
			for (int32 Index = 0; Index < Random; ++Index)
			{
				if (AvailableSlotsIndex.IsEmpty())
				{
					break;
				}
				const int32 RandomIndex = FMath::RandRange(0, AvailableSlotsIndex.Num() - 1);
				Targets.Add(EnemyTeam[AvailableSlotsIndex[RandomIndex]]);
				AvailableSlotsIndex.RemoveAt(RandomIndex);
			}
			break;
		}
		case ECombatActionFlow::WHOLE_TEAM_IN_PLACE:
		{
			for (const auto& Slot : PlayerTeam)
			{
				if (Slot && Slot->IsActive())
				{
					Targets.Add(Slot);
				}
			}
			break;
		}
		default:
		{
			return;
		}
		
	}

	AddActionToQueue(EnemySlot, Targets, ActionData, true);
}
```

</br>**General:** The queue stores all used actions. The manager calls the queue function every 0.5 seconds and checks if no action is currently being executed. If so, it stops all cooldown bars and executes the current action. This allows actions to be queued and executed in a proper time.

```c++
void ABLCombatManager::HandleActionsQueue()
{
	// Do nothing if other action is currently performed. 
	if (bAction || ActionQueue.IsEmpty())
	{
		return;
	}

	// Delete action from queue if owner is not active (dead).
	if (!IsValid(ActionQueue[0].OwnerSlot) || !ActionQueue[0].OwnerSlot->IsActive())
	{
		ActionQueue.RemoveAt(0);
		bAction = false;
		return;
	}
		
	// If it's not summon allies type of action, check if all targets are active (alive). If not, find new active target.
	if (ActionQueue[0].ActionData.Flow != ECombatActionFlow::SUMMON_ALLIES)
	{
		for (int32 Index = ActionQueue[0].TargetsSlots.Num() - 1; Index >= 0; --Index)
		{
			if (ActionQueue[0].TargetsSlots[Index] && !ActionQueue[0].TargetsSlots[Index]->IsActive())
			{
				if (ABLCombatSlot* NewTargetSlot = FindNewTargetSlot(ActionQueue[0].bEnemyAction))
				{
					ActionQueue[0].TargetsSlots[Index] = NewTargetSlot;
				}
				else
				{
					ActionQueue[0].TargetsSlots.RemoveSingle(ActionQueue[0].TargetsSlots[Index]);
				}
			}
		}
	}

	DoAction(ActionQueue[0].OwnerSlot, ActionQueue[0].TargetsSlots, ActionQueue[0].ActionData, ActionQueue[0].bEnemyAction);
	ActionQueue.RemoveAt(0);
}
```

</details>

# UI ([code](Source/BladeOfLegend/DAWID/UI))
<details>
<summary>More</summary>
	
</br>The player controls the gameplay by interacting with the UI using the mouse. Therefore, it is very important that the UI elements work properly. While the visual elements are done in blueprints, all logic is done entirely in cpp to maintain performance and proper integration with the system.
</br></br>The main panel contains 3 elements: **Enemies Panel** (displays information about enemies, not interactive), **Heroes Panel** (displays information about heroes, by clicking on them we can switch the currently active Hero), **Actions Panel** (contains information about all actions, we can choose there which one we want to use).

![UIgif](https://github.com/user-attachments/assets/3b1be6e0-4624-469b-b475-c354622e42cd)

</br>For example, the logic responsible for selecting the currently active hero starts in **BLHeroesWidget**. We add a function delegate to the **On click event**, which highlights the tile and calls another delegate that is binded in the **BLComabtWidget**. Then, in the same way, we go to the **Combat manager** were CombatWidget delegates are binded, and select the corresponding player slot.

```c++
void UBLHeroesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HeroesTileView->OnItemClicked().AddUObject(this, &UBLHeroesWidget::HeroClicked);
}

void UBLHeroesWidget::HeroClicked(UObject* Item)
{
	UBLHeroEntryWidget* Hero = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(Item));
	if (Hero && Hero->CanDoAction())
	{
		UnlightsHero();
		ClickedHero = Hero;
		ClickedHero->HighlightHero();
		OnHeroClicked.ExecuteIfBound(Hero->GetIndex());
	}
}
```

```c++
void UBLCombatWidget::AddHero(int32 SlotIndex, const FCombatCharData& BaseData, bool bSneakAttack)
{
	Heroes->AddHero(SlotIndex, BaseData.Name, BaseData.MaxHP, BaseData.CurrentHP, BaseData.MaxME);
	Heroes->OnHeroClicked.BindUObject(this, &UBLCombatWidget::HeroClicked);

	.
	.
	.
}

void UBLCombatWidget::HeroClicked(int32 HeroIndex)
{
	OnHeroSelected.ExecuteIfBound(HeroIndex);
}
```

```c++
void ABLCombatManager::InitializeWidget()
{
	if (WidgetClass)
	{
		Widget = CreateWidget<UBLCombatWidget>(GetWorld(), WidgetClass);
		Widget->OnActionChosen.BindUObject(this, &ABLCombatManager::SetPlayerActionData);
		Widget->OnHeroSelected.BindUObject(this, &ABLCombatManager::ChoosePlayerSlot);
		Widget->OnResetCurrentActionType.BindWeakLambda(this, [this]() {CurrentActionType = ECombatActionType::NONE; });
	}
}

void ABLCombatManager::ChoosePlayerSlot(int32 Index)
{
	if (PlayerTeam.IsValidIndex(Index))
	{
		ClearPlayerSlot();
		ChoosePlayerSlot(PlayerTeam[Index]);
	}
}
```

</details>
