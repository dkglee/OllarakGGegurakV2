// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomGameMode.h"

#include "MediaPlayer.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/ClientRoomGameState.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "JumpGame/Maps/Node/StageMapNodeComponent.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "JumpGame/UI/ClientRoomLogoUI.h"
#include "JumpGame/UI/Cinematic/Ending.h"
#include "JumpGame/UI/Cinematic/IntroCinematic.h"
#include "JumpGame/UI/Cinematic/OutroCinematic.h"
#include "JumpGame/Utils/FastLogger.h"


void AClientRoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerClass = ALobbyPlayerController::StaticClass();

	// 게임 인스턴스 가져오기
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	UStageSystemSubsystem* SubGI = GI->GetSubsystem<UStageSystemSubsystem>();
	SubGI->SetChosenStage(TEXT("Stage_01"));		
	
	// TODO: GI 초기화
	GI->GetPlayerInfo().Empty();

	GI->CurrentMap = EMapKind::Lobby;
	UE_LOG(LogTemp, Warning, TEXT("CurrentMap: %d"), GI->CurrentMap);

	if (!GI->bIsGameStart)
	{
		IntroCinematic = CreateWidget<UIntroCinematic>(GetWorld(), IntroCinematicUIClass);
		if (IntroCinematic)
		{
			IntroCinematic->AddToViewport(100);
			IntroCinematic->MediaPlayer->OnEndReached.AddDynamic(this, &AClientRoomGameMode::OnVideoEnd);
		}
	}
	
	if (GI->bLastMapClear)
	{
		OutroCinematic = CreateWidget<UOutroCinematic>(GetWorld(), OutroCinematicUIClass);
		if (OutroCinematic)
		{
			OutroCinematic->AddToViewport(100);
			UE_LOG(LogTemp, Warning, TEXT("OutroCinematic Add1111111111111"));
			OutroCinematic->MediaPlayer->OnEndReached.AddDynamic(this, &AClientRoomGameMode::OnOutroVideoEnd);
		}
	}
	
	if (UStageMapNodeComponent* NodeComp = UStageMapNodeComponent::Get(GetWorld()))
	{
		NodeComp->InitAllFieldStars();
	}
}

void AClientRoomGameMode::OnVideoEnd()
{
	if (IntroCinematic)
	{
		// 두번 호출 되므로 바인딩 삭제
		IntroCinematic->MediaPlayer->OnEndReached.RemoveAll(this);
		IntroCinematic->RemoveFromParent();
		ClientRoomLogoUI = CreateWidget<UClientRoomLogoUI>(GetWorld(), ClientRoomLogoUIClass);
		if (ClientRoomLogoUI)
		{
			ClientRoomLogoUI->AddToViewport(20);
			GI->bIsGameStart = true;
		}
	}
}

void AClientRoomGameMode::OnOutroVideoEnd()
{
	if (OutroCinematic)
	{
		OutroCinematic->MediaPlayer->OnEndReached.RemoveAll(this);
		OutroCinematic->RemoveFromParent();
		EndingUI = CreateWidget<UEnding>(GetWorld(), EndingUIClass);
		if (EndingUI)
		{
			EndingUI->AddToViewport(40);
		}
	}
}
