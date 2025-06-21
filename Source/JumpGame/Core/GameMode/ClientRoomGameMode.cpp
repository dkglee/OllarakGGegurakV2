// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomGameMode.h"

#include "MediaPlayer.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/ClientRoomGameState.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "JumpGame/UI/ClientRoomLogoUI.h"
#include "JumpGame/UI/Cinematic/IntroCinematic.h"
#include "JumpGame/Utils/FastLogger.h"


void AClientRoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	FFastLogger::LogFile(TEXT("ClientRoomGameMode"), 
		TEXT("BeginPlay called."));

	PlayerControllerClass = ALobbyPlayerController::StaticClass();

	// 게임 인스턴스 가져오기
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	
	FFastLogger::LogFile(TEXT("ClientRoomGameMode"), 
		TEXT("Middle called."));
	// TODO: GI 초기화
	GI->GetPlayerInfo().Empty();

	if (GI->bIsGameStart)
	{
		FFastLogger::LogFile(TEXT("ClientRoomGameMode"), 
			TEXT("Early called."));
		return;
	}
	
	IntroCinematic = CreateWidget<UIntroCinematic>(GetWorld(), IntroCinematicUIClass);
	if (IntroCinematic)
	{
		IntroCinematic->AddToViewport(100);
		IntroCinematic->MediaPlayer->OnEndReached.AddDynamic(this, &AClientRoomGameMode::OnVideoEnd);
	}
	
	FFastLogger::LogFile(TEXT("ClientRoomGameMode"), 
		TEXT("End called."));
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