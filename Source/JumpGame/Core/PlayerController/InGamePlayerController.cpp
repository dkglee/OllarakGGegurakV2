// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "JumpGame/UI/InGame/InGameUI.h"
#include "JumpGame/UI/InGame/StageResultUI.h"
#include "Components/WidgetComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/UI/InGame/OutToMainUI.h"
#include "JumpGame/Utils/FastLogger.h"

AInGamePlayerController::AInGamePlayerController()
{
	ConstructorHelpers::FClassFinder<UInGameUI> InGameUIClassFinder
		(TEXT("/Game/UI/Game/WBP_InGame.WBP_InGame_C"));
	if (InGameUIClassFinder.Succeeded())
	{
		InGameUIClass = InGameUIClassFinder.Class;
	}

	ConstructorHelpers::FClassFinder<UStageResultUI> StageResultUIClassFinder
		(TEXT("/Game/UI/Game/WBP_StageResult.WBP_StageResult_C"));
	if (StageResultUIClassFinder.Succeeded())
	{
		ResultUIClass = StageResultUIClassFinder.Class;
	}

	ConstructorHelpers::FClassFinder<UOutToMainUI> OutToMainUIClassFinder
	(TEXT("/Game/UI/Game/WBP_OutToMain.WBP_OutToMain_C"));
	if (OutToMainUIClassFinder.Succeeded())
	{
		OutToMainUIClass = OutToMainUIClassFinder.Class;
	}
}

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	StartInitialize();

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);

	// 인게임 UI 생성
	InGameUI = CreateWidget<UInGameUI>(this, InGameUIClass);
	if (InGameUI)
	{
		InGameUI->AddToViewport();
	}

	ResultUI = CreateWidget<UStageResultUI>(this, ResultUIClass);
	OutToMainUI = CreateWidget<UOutToMainUI>(this, OutToMainUIClass);
}

void AInGamePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	StartInitialize();
}

void AInGamePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	StartInitialize();
}

void AInGamePlayerController::StartInitialize()
{
	AFrog* Frog{Cast<AFrog>(GetPawn())};
	if (!Frog)
	{
		return;
	}

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);

	Frog->InitJumpGaugeUIComponent();
}

void AInGamePlayerController::ShowOutToMainUI()
{
	if (InGameUI)
	{
		InGameUI->RemoveFromParent();
	}

	if (OutToMainUI)
	{
		OutToMainUI->AddToViewport();
	}
}

void AInGamePlayerController::ShowResultUI()
{
	if (InGameUI)
	{
		InGameUI->RemoveFromParent();
	}
	if (ResultUI)
	{
		ResultUI->AddToViewport();
	}
}

void AInGamePlayerController::UpdateStarCount(int32 Count)
{
	if (InGameUI)
	{
		InGameUI->UpdateStarCount(Count);
	}
}
