// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SRGEditorTarget : TargetRules
{
	public SRGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		// IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		// ExtraModuleNames.Add("SRG");
		
		ExtraModuleNames.AddRange( new string[]
		{
			"SRGCore","SRG"
		} );
	}
}
