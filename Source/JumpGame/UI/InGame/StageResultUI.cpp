// Fill out your copyright notice in the Description page of Project Settings.


#include "StageResultUI.h"

#include "Components/Button.h"
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

	PlayAnimation(ClearAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
}

void UStageResultUI::OnClickOutToMain()
{
	// Todo: 나중에 멀티 고려하면 server travel로 ?

	FString CurrentLevelName{UGameplayStatics::GetCurrentLevelName(GetWorld(), true)};
	if (CurrentLevelName == TEXT("InGameLevel"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/ClientRoomLevel"));
	}
	else
	{
		FLog::Log("Not InGameLevel, Can not Go");
	}
}
