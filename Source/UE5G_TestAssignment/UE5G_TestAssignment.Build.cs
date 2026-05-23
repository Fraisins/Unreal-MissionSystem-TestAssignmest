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

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate", // UI
			"SlateCore",
		});
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
