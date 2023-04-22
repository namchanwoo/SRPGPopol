// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SRG : ModuleRules
{
	public SRG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput",
			"UMG","UMGEditor",
			"SRGCore"
		});
        
		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}