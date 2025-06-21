// Fill out your copyright notice in the Description page of Project Settings.


#include "StartSoundUI.h"

void UStartSoundUI::PlayStartSoundAnim()
{
	if (RecordMovement)
	{
		PlayAnimation(RecordMovement);
	}
}
