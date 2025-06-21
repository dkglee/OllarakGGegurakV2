// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JumpGame : ModuleRules
{
	public JumpGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"UMG", "OnlineSubsystem" , "OnlineSubsystemSteam", "SlateCore",
			"Json", "JsonUtilities", "HTTP", "WebSockets", "InteractiveToolsFramework",
			"MotionTrajectory", "AdvancedWidgets", "AudioMixer", "CableComponent", "MediaAssets",
			"ImageWrapper", "RenderCore", "RHI"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		
		// 에디터 전용 모듈은 조건부로 추가
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd"
			});
		}
	}
}
