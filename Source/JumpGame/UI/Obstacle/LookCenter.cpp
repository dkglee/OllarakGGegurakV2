// Fill out your copyright notice in the Description page of Project Settings.


#include "LookCenter.h"

#include "Animation/WidgetAnimation.h"

void ULookCenter::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULookCenter::TextMoveAnimation()
{
	if (TextMovement)
	{
		PlayAnimation(TextMovement, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	}
}
