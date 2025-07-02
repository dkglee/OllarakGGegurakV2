// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeTutorialUI.h"

#include "JumpGame/UI/UICam/LobbyCameraComp.h"

void UNodeTutorialUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	GetWorld()->GetTimerManager().SetTimer(TutorialAnimTimer, this, &UNodeTutorialUI::PlayTutorialAnim, 2.0f, true);
}

void UNodeTutorialUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
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
