// Fill out your copyright notice in the Description page of Project Settings.


#include "WinnerPlayerUI.h"

void UWinnerPlayerUI::PlayWinnerAnim()
{
	PlayAnimation(WinnerClearAnim, 0,1, EUMGSequencePlayMode::Forward, 1.5);
}
