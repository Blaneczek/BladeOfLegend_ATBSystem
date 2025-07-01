# Active Time Battle System
Active Time Battle is a type of turn-based combat system in which each character has its own time bar to attack at its own pace. There is no pause for your action, as in the classic turn-based system.

![atb_system](https://github.com/user-attachments/assets/dd45f40c-6826-458c-b025-c6fe10f13a89)

# Introduction
The system was created while participating in a game jam. The team and I were developing a classic 2D RPG with turn-based combat. The game was made using Unreal Engine 5 and C++. My task was to create the combat system and implement the designers' ideas, as well as integrate the system with the game's UI. The system consists of 6 types of activities: attack, defense, crystal skills, weapon skills, items, escape. Each of them uses actions which are templates for skills which can be easily created in 9 different prepared types. The video demonstrates a sample gameplay showing most of the elements of the system: (https://www.youtube.com/watch?v=getVlf-3iAM or click the image below)

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
Actions are used for all activities that characters perform during combat, from using abilities to using items. Each action is a UObject that is created when any activity is performed. There are 9 types of actions that determine how an activity will be performed: </br>
  
![image](https://github.com/user-attachments/assets/b5b246d9-91e8-4e83-b154-97bad793ce8a)

**Default:** an action that is used on the character who performs it, such as using items or the Defense ability.
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

</br>Let's take a look at how this works. For example, the player selects Attack, selects the enemy character, and then the proper UObject is created. An action of type DefaultMelee is used for this purpose. The character's default attack is created in Blueprint, which inherits from the DefaultMeleeAction class. This way designers can easily create abilities in Blueprints, set all the necessary data and calculate damage or other effects. 
  
![image](https://github.com/user-attachments/assets/5f530f65-0e81-433f-a5ac-ebcfae6f0f37)

</br>Actions are managed by BLActionComponent, which is added to Characters. In this component, the correct action type is first created in the CreateAction function based on ActionData.Type. Then the OnCreateAction function of the newly created UObject is called.

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

</br>The ActivateAction function is then called in the Action object. Each action type has its own implementation of this function. The action we chose earlier is of type DefaultMeleeAction. There we return to the ActionComponent and call the DefaultMeleeAction function. 

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

</br>ActionComponent manages all the action flows. In our example, the DefaultMeleeAction function is called, which calculates the location where our character needs to move and sends it there.

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

</details>

# Core ([code](Source/BladeOfLegend/DAWID/Core)) 
<details>
<summary>More</summary>

</details>

# UI ([code](Source/BladeOfLegend/DAWID/UI))
<details>
<summary>More</summary>

</details>
