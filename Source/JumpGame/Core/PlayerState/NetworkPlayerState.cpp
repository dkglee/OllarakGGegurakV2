// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerState.h"

#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Utils/FastLogger.h"

void ANetworkPlayerState::OnSetUniqueId()
{
	Super::OnSetUniqueId();

	bIsSetUniqueId = true;
}

bool ANetworkPlayerState::ConnectToServer()
{
	if (!bIsSetUniqueId)
	{
		FFastLogger::LogConsole(TEXT("Player %s is not set UniqueID"), *GetName());
		return false;
	}
	// 로컬 플레이어 컨트롤러를 가져온다.
	// 검사
	bool bIsLocalPlayer = false;
	
	// 서버에 연결되었을 때 서버에 자신의 NetID를 전송한다.
	const FUniqueNetIdRepl& PlayerNetID = GetUniqueId();
	FUniqueNetIdPtr PlayerNetIDPtr = PlayerNetID.GetUniqueNetId();
	
	bIsLocalPlayer |= (GetLocalRole() == ROLE_Authority && GetRemoteRole() == ROLE_SimulatedProxy);
	// ?? 왜 SimulatedProxy이지? AutonomousProxy가 아닌 이유는? // 조종하는 객체가 아니기 때문이다.!! 하지만 Owner는 클라이언트로 되어 있음!
	bIsLocalPlayer |= (GetLocalRole() == ROLE_SimulatedProxy && GetRemoteRole() == ROLE_Authority);

	FFastLogger::LogConsole(TEXT("GetLocalRole : %d && GetRemoteRole : %d"), GetLocalRole(), GetRemoteRole());
	if (!bIsLocalPlayer)
	{
		FFastLogger::LogConsole(TEXT("Player %s is not local player : %s"), *GetName(), *PlayerNetIDPtr->ToString());
		return false;
	}
		
	if (PlayerNetIDPtr.IsValid())
	{
		FFastLogger::LogConsole(TEXT("Player %s NetID is valid : %s"), *GetName(), *PlayerNetIDPtr->ToString());
		FString NetID = PlayerNetIDPtr->ToString();
		ServerRPC_HandShake(NetID);
		return true;
	}
	else
	{
		FFastLogger::LogConsole(TEXT("Player %s NetID is not valid"), *GetName());
		return false;
	}
}

void ANetworkPlayerState::ServerRPC_HandShake_Implementation(const FString& NetID)
{
	ANetworkGameState* GameState = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		FFastLogger::LogConsole(TEXT("ServerRPC Player NetID is valid : %s"), *NetID);
		GameState->HandleConnection(NetID);
	}
}

void ANetworkPlayerState::ServerRPC_ConnectionVerified_Implementation(const FString& NetID)
{
	ANetworkGameState* GameState = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->VerifyConnection(NetID);
	}
}
