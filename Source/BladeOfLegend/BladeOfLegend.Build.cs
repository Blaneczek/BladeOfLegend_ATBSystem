// Copyright (c) 2023 Smoking Carrots. All rights reserved.

using UnrealBuildTool;

public class BladeOfLegend : ModuleRules
{
	public BladeOfLegend(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PaperZD", "Paper2D", "AIModule", "EnhancedInput", "Slate", "SlateCore", "LevelSequence" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] { "BladeOfLegend/DAWID/CombatSystem", "BladeOfLegend/DAWID/Items" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}