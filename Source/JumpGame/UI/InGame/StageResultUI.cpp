// Fill out your copyright notice in the Description page of Project Settings.


#include "StageResultUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

void UStageResultUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OutToMainButton->OnClicked.AddDynamic(this, &UStageResultUI::OnClickOutToMain);
}

void UStageResultUI::NativeConstruct()
{
	Super::NativeConstruct();

	AMapGameState* MapGameState = Cast<AMapGameState>(GetWorld()->GetGameState());
	if (!MapGameState)
	{
		FFastLogger::LogConsole(TEXT("StageResultUI::NativeConstruct - MapGameState is nullptr"));
		return;
	}

	int32 Min = static_cast<int32>(MapGameState->ClearTime / 60);
	int32 Sec = static_cast<int32>(MapGameState->ClearTime) % 60;
	FString ClearTimeText = FString::Printf(TEXT("%02d:%02d"), Min, Sec);
	Text_ClearTime->SetText(FText::FromString(ClearTimeText));
	
	PlayAnimation(ClearAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
}

void UStageResultUI::OnClickOutToMain()
{
	// Todo: 나중에 멀티 고려하면 server travel로 ?

	if (GetWorld()->GetMapName().Contains(TEXT("Stage")))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/Levels/GameLobby?AutoStartStage=1"));
	}
	else
	{
		FLog::Log("Not Stage, Can not Go");
	}
}
