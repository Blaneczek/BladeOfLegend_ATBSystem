// Copyright (c) 2023 Smoking Carrots. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BladeOfLegendEditorTarget : TargetRules
{
	public BladeOfLegendEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("BladeOfLegend");
	}
}