// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/PlayerState.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "JumpGame/Utils/FastLogger.h"

ALobbyGameMode::ALobbyGameMode()
{
	// GamaeState, PlayerState 설정하기
	DefaultPawnClass = nullptr;
	GameStateClass = ALobbyGameState::StaticClass();
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 게임 인스턴스 가져오기
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());

	FString Key;
	FString SteamName = Key; // 스팀없을때 기본값
	
	// 플레이어의 고유한 네트워크 ID값 가져오기
	const FUniqueNetIdRepl& NetIdRepl = NewPlayer->GetPlayerState<APlayerState>()->GetUniqueId();
	if (!NetIdRepl.IsValid()) return;
	
	TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
	if (!NetId.IsValid()) return;
	
	// Steam 닉네임이 있다면 사용
	if (IOnlineSubsystem* Subsys = IOnlineSubsystem::Get())
	{
		IOnlineIdentityPtr Identity = Subsys->GetIdentityInterface();
		if (Identity.IsValid())
		{
			SteamName = Identity->GetPlayerNickname(*NetId);
		}
	}
	
	// 이미 등록되어 있다면 리턴
	if (GI->GetPlayerInfo().Contains(SteamName)) return;
	
	// FFastLogger::LogConsole(TEXT("ALobbyGameMode PlayerIdx: %d"), PlayerIdx);
	
	// 새 플레이어 정보 생성 및 저장
	FPlayerInfo NewPlayerInfo;
	NewPlayerInfo.PlayerID = PlayerIdx;
	NewPlayerInfo.PlayerName = SteamName;

	GI->AddPlayerInfo(SteamName, NewPlayerInfo);
	PlayerIdx++;
}
