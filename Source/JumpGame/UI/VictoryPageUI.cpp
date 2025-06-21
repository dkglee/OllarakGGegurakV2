// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPageUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

void UVictoryPageUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Btn_GoLobby->OnClicked.AddDynamic(this, &UVictoryPageUI::OnClickGoLobby);

	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UVictoryPageUI::OnClickGoLobby()
{
	// 서버가 나가면 다같이 나가기
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (!GI || !PC) return; 
	if (!PC->HasAuthority()) return;

	// 클라이언트들 먼저 이동시킴
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* OtherPC = Cast<APlayerController>(*It);
		if (OtherPC && !OtherPC->IsLocalController())
		{
			OtherPC->ClientTravel(TEXT("/Game/Maps/ClientRoomLevel?closed"), TRAVEL_Absolute);
		}
	}

	// 서버도 세션 제거하고 이동
	GI->LeaveSession(true);

	// GetWorld()->ServerTravel(TEXT("/Game/Maps/ClientRoomLevel?listen"));
	FLog::Log("Server Leaving Game, 로비로 이동");
		
	/*if (!PC->HasAuthority())
	{
		// 클라이언트는 이동 후 세션 나가기
		PC->ClientTravel(TEXT("/Game/Maps/ClientRoomLevel?closed"), TRAVEL_Absolute);
		
		GI->LeaveSession(true);
	}
	else
	{
		// 클라이언트들 먼저 이동시킴
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* OtherPC = Cast<APlayerController>(*It);
			if (OtherPC && !OtherPC->IsLocalController())
			{
				OtherPC->ClientTravel(TEXT("/Game/Maps/ClientRoomLevel?closed"), TRAVEL_Absolute);
			}
		}

		// 서버도 세션 제거하고 이동
		GI->LeaveSession(true);

		GetWorld()->ServerTravel(TEXT("/Game/Maps/ClientRoomLevel?listen"));
		FLog::Log("Server Leaving Game, 로비로 이동");
	}*/
	
	// 이동하면 나는 지우자
	RemoveFromParent();
}

void UVictoryPageUI::SetVictoryPlayerName(FString PlayerName)
{
	// WinnerPlayer 이름 업데이트
	Text_PlayerName->SetText(FText::FromString(PlayerName));
}

void UVictoryPageUI::PlayWinnerPageAnim()
{
	PlayAnimation(WinnerAnim);
}
