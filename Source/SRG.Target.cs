// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SRGTarget : TargetRules
{
	public SRGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		// IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		// ExtraModuleNames.Add("SRG");
		
		
		ExtraModuleNames.AddRange( new string[]
		{
			"SRGCore","SRG"
		} );
	}
}