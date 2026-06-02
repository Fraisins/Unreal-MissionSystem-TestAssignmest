// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5G_TestAssignment : ModuleRules
{
	public UE5G_TestAssignment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"UMG", // UI
		});
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] 
			{
				"UMGEditor",
				"UnrealEd" 
			});
		}
	}
}
