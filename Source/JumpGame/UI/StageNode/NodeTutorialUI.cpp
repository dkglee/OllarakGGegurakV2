// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeTutorialUI.h"

void UNodeTutorialUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimer(TutorialAnimTimer, this, &UNodeTutorialUI::PlayTutorialAnim, 2.0f, true);
}

void UNodeTutorialUI::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TutorialAnimTimer);
	}
	
	Super::NativeDestruct();
}

void UNodeTutorialUI::PlayTutorialAnim()
{
	PlayAnimation(TutorialAnim);
}
