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

Default: an action that is used on the character who performs it, such as using items or the Defense ability.

</br>Default Melee/Range: an action that is used on another character, such as using the default attack ability. In Melee the character runs to the target, in Range the character sends a projectile to the target.
</br>![meleedefault-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/e1b230ab-d648-427a-884e-e21447facafa) ![meleedefault-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/e1b230ab-d648-427a-884e-e21447facafa) 


</br>Multiple Default Melee/Range: an action used on multiple characters.
</br>Bounce Range: a special action that sends a single projectile that bounces between multiple characters.
</br>Column Melee: a special action in which one column of slots is selected and the character performs the ability on every character in that column
</br>Multiple In Place: a special action in which the ability is used on multiple characters without projectiles or having to walk up to them..
</br>Summon: a special action that allows you to summon new characters to slots.


</br>Let's take a look at how this works. For example, the player selects Attack, selects the enemy character, and then the proper UObject is created. An action of type DefaultMelee is used for this purpose. The character's default attack is created in Blueprint, which inherits from the DefaultMeleeAction class. This way designers can easily create abilities in Blueprints, set all the necessary data and calculate damage or other effects. 
  
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
