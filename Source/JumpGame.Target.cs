// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class JumpGameTarget : TargetRules
{
	public JumpGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		//
		// BuildEnvironment = TargetBuildEnvironment.Unique; // ★ 이 줄 추가
		//
		// bUseLoggingInShipping = true; // ★ Shipping 빌드에서도 로그 출력 가능
		
		ExtraModuleNames.Add("JumpGame");
	}
}
