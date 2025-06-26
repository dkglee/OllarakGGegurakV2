// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"

#include "Animation/UMGSequencePlayer.h"
#include "JumpGame/Core/PlayerController/InGamePlayerController.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

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

void UInGameUI::PlayClearAnimation()
{
	UUMGSequencePlayer* SequencePlayerClear = PlayAnimation(ClearAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	TWeakObjectPtr<UInGameUI> WeakThis{this};
	SequencePlayerClear->OnSequenceFinishedPlaying().AddLambda([WeakThis](UUMGSequencePlayer&) {
		if (!WeakThis.IsValid())
		{
			return;
		}
		
		UInGameUI* StrongThis{WeakThis.Get()};
		AInGamePlayerController* PC{(Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(StrongThis, 0)))};
		PC->ShowResultUI();
	});
}

void UInGameUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
