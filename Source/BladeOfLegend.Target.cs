// Copyright (c) 2023 Smoking Carrots. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BladeOfLegendTarget : TargetRules
{
	public BladeOfLegendTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("BladeOfLegend");
	}
}