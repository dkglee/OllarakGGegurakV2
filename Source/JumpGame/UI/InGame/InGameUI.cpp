// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"

#include "JumpGame/Utils/FastLogger.h"

void UInGameUI::UpdateStarCount(int32 Count)
{
	FLog::Log("Star", Count);
}

void UInGameUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
