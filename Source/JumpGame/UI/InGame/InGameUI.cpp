// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"

#include "JumpGame/Utils/FastLogger.h"

void UInGameUI::UpdateStarCount(int32 Count)
{
	switch (Count)
	{
	case 1:
		PlayAnimation(StarAnimation1, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
		break;
	case 2:
		PlayAnimation(StarAnimation2, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
		break;
	case 3:
		PlayAnimation(StarAnimation3, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
		break;
	default:
		break;
	}
}

void UInGameUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
