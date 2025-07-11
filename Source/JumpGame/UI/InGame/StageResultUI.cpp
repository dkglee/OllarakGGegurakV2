// Fill out your copyright notice in the Description page of Project Settings.


#include "StageResultUI.h"

#include "MediaPlayer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "JumpGame/UI/Cinematic/OutroCinematic.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

class UStageSystemSubsystem;

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
		UStageSystemSubsystem* SGI = GetWorld()->GetGameInstance()->GetSubsystem<UStageSystemSubsystem>();
		if (SGI->GetChosenField() == FString(TEXT("Field_01_10")))
		{
			UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
			if (GI)
			{
				GI->bLastMapClear = true;
				UE_LOG(LogTemp, Warning, TEXT("GI->bLastMapClear = true"));
			}
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/Levels/GameLobby?AutoStartStage=1"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GI->bLastMapClear = false"));
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/Levels/GameLobby?AutoStartStage=1"));
		}
	}
	else
	{
		FLog::Log("Not Stage, Can not Go");
	}
}
