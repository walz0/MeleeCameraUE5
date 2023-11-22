// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MeleeCameraUE5 : ModuleRules
{
	public MeleeCameraUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
