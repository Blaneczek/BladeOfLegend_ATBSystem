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
Actions are used for all activities that characters perform during combat from using abbilities to using items. Each action is a UObject that is created while perform any activity. There are 9 types of action telling how activity will be performed: </br>
  
![image](https://github.com/user-attachments/assets/b5b246d9-91e8-4e83-b154-97bad793ce8a)

Default: its an action that is used on Character that performs it, e.g. using items or Defend ability.
</br>Default Melee/Range: its an action used on the other character, e.g. using default Attack ability.
</br>Multiple Default Melee/Range: its an action used on many characters.
</br>Bounce Range: special action that sends one projectile that bounce beetween multiple characters.
</br>Column Melee: special action where one column of slots are chose and character perform ability on each character from that column
</br>Multiple In Place: special action where ability is used on many characters without projectiles or needing to come to them.
</br>Summon: special action that allows summoning new characters to slots.


</br>Let's dive in how this works. For example Player choses Attack, marks enemy character and then proper UObject is created. For that, action of type DefaultMelee is used. Default attack of the Hero is created in Blueprint that inherits from DefaultMeleeAction class. That way designers can easly create abilities in blueprints, set up all neccesery data and calculations od damage or other effects. 
  
![image](https://github.com/user-attachments/assets/5f530f65-0e81-433f-a5ac-ebcfae6f0f37)

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
