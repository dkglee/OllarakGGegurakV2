// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameState.h"

#include "GameFramework/PlayerState.h"
#include "JumpGame/AIServices/Shared/IOManagerComponent.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/PlayerController/NetworkPlayerController.h"
#include "JumpGame/Core/PlayerState/NetworkPlayerState.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"
#include "JumpGame/Utils/FastLogger.h"

class ANetworkPlayerState;

ANetworkGameState::ANetworkGameState()
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = true;

	ConnectionVerifyComponent = CreateDefaultSubobject<UConnectionVerifyComponent>(TEXT("ConnectionVerifyComponent"));
	ConnectionVerifyComponent->SetNetAddressable();
	ConnectionVerifyComponent->SetIsReplicated(true);

	// 서버에서 호출됨
	ConnectionVerifyComponent->OnClientAdded.AddDynamic(this, &ANetworkGameState::OnClientAdded);
	ConnectionVerifyComponent->OnAllClientAdded.AddDynamic(this, &ANetworkGameState::OnAllClientAdded);
	// 클라이언트에서 호출됨
	ConnectionVerifyComponent->OnConnectionSucceeded.AddDynamic(this, &ANetworkGameState::OnConnectionSucceeded);
	ConnectionVerifyComponent->OnConnectionBlocked.AddDynamic(this, &ANetworkGameState::OnConnectionBlocked);

	// TODO: 초기화는 각각의 맵에서 진행하는걸로 한다.
	IOManagerComponent = CreateDefaultSubobject<UIOManagerComponent>(TEXT("IOManagerComponent"));
}


double ANetworkGameState::GetServerWorldTimeSeconds() const
{
	// 기존의 시간을 가져오는 함수를 사용하지 않고, 서버 시간을 가져오도록 변경한다.
	// return Super::GetServerWorldTimeSeconds();

	// 서버 자기 자신이 요청하는 경우 서버 시간을 반환한다.
	if (GetLocalRole() == ROLE_Authority)
	{
		return ServerWorldTimeSeconds;
	}

	// 클라이언트가 요청하는 경우 클라이언트가 서버와 직접 동기화된 시간을 가져온다.
	// FirstPlayerController를 가져오는 이유는 해당 플레이어 컨트롤러에서만 관리되기 때문이다.
	if(ANetworkPlayerController* PC = Cast<ANetworkPlayerController>(GetGameInstance()->GetFirstLocalPlayerController(GetWorld())))
	{
		return PC->GetServerTime();
	}
	else
	{
		return GetWorld()->GetTimeSeconds();
	}
}

void ANetworkGameState::TryConnectToServer()
{
	bool bRetry = false;
	ANetworkPlayerController* PlayerController = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
	{
		FFastLogger::LogConsole(TEXT("PlayerController is null"));
		bRetry |= true;
	}

	ANetworkPlayerState* PlayerState = Cast<ANetworkPlayerState>(PlayerController->PlayerState);
	if (PlayerState == nullptr)
	{
		FFastLogger::LogConsole(TEXT("PlayerState is null"));
		bRetry |= true;
	}

	if (bRetry || !PlayerState->ConnectToServer())
	{
		// 재시도
		GetWorld()->GetTimerManager().SetTimer(ConnectionTimer, this, &ANetworkGameState::TryConnectToServer,
			CheckInterval, false);
		return;
	}
}

void ANetworkGameState::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(ConnectionTimer, this, &ANetworkGameState::TryConnectToServer,
		CheckInterval, false);
}

void ANetworkGameState::VerifyConnection(const FString& NetID)
{
	ConnectionVerifyComponent->ConfirmClient(NetID);
}

void ANetworkGameState::HandleConnection(const FString& NetID)
{
	ConnectionVerifyComponent->AddClient(NetID);
	MulticastRPC_ConnectionSucceeded(NetID);
}

void ANetworkGameState::MulticastRPC_ConnectionSucceeded_Implementation(const FString& NetID)
{
	ANetworkPlayerController* PlayerController = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
	{
		FFastLogger::LogConsole(TEXT("PlayerController is null"));
		return;
	}
	ANetworkPlayerState* PlayerState = Cast<ANetworkPlayerState>(PlayerController->PlayerState);
	if (PlayerState == nullptr)
	{
		FFastLogger::LogConsole(TEXT("PlayerState is null"));
		return;
	}
	
	// 서버에 연결되었을 때 서버에 자신의 NetID를 전송한다.
	const FUniqueNetIdRepl& PlayerNetID = PlayerState->GetUniqueId();
	FUniqueNetIdPtr PlayerNetIDPtr = PlayerNetID.GetUniqueNetId();
	FString MyNetID = PlayerNetIDPtr->ToString();
	FFastLogger::LogConsole(TEXT("My NetID : %s"), *MyNetID);
	FFastLogger::LogConsole(TEXT("ConnectedNetID : %s"), *NetID);
	if (NetID == MyNetID)
	{
		FFastLogger::LogConsole(TEXT("GameState Connection Succeeded : %s"), *NetID);
		// 자신의 NetID와 같다면 연결이 성공한 것이다.
		ConnectionVerifyComponent->SetClientVerify(true);
		PlayerState->ServerRPC_ConnectionVerified(MyNetID);
	}
}

// 클라이언트 한명이 연결에 성공했을 때 서버에서 호출되는 함수. NetID는 연결된 클라이언트의 NetID이다.
void ANetworkGameState::OnClientAdded(const FString& NetID)
{
	FFastLogger::LogConsole(TEXT("GameState Client Added : %s"), *NetID);
}

// 모든 클라이언트가 연결되었을 때 서버에서 호출되는 함수.
void ANetworkGameState::OnAllClientAdded()
{
	FFastLogger::LogConsole(TEXT("GameState All Client Added"));
}

// 서버와 성공적으로 연결이 되었을 때 클라이언트에서 호출되는 함수.
void ANetworkGameState::OnConnectionSucceeded()
{
	FFastLogger::LogConsole(TEXT("GameState Connection Succeeded"));
}

// 연결이 끊길 때 호출되는 함수.
void ANetworkGameState::OnConnectionBlocked()
{
	FFastLogger::LogConsole(TEXT("GameState Connection Blocked"));
}

void ANetworkGameState::MulticastRPC_RetryConnections_Implementation(const TArray<FString>& NetIDs)
{
	ANetworkPlayerController* PlayerController = Cast<ANetworkPlayerController>(GetWorld()->GetFirstPlayerController());
	ANetworkPlayerState* PlayerState = Cast<ANetworkPlayerState>(PlayerController->PlayerState);
	
	// 서버에 연결되었을 때 서버에 자신의 NetID를 전송한다.
	const FUniqueNetIdRepl& PlayerNetID = PlayerState->GetUniqueId();
	FUniqueNetIdPtr PlayerNetIDPtr = PlayerNetID.GetUniqueNetId();
	FString MyNetID = PlayerNetIDPtr->ToString();
	if (NetIDs.Contains(MyNetID))
	{
		// 자신의 NetID와 같다면 연결이 성공한 것이다.
		ConnectionVerifyComponent->SetClientVerify(true);
		PlayerState->ServerRPC_ConnectionVerified(MyNetID);
	}
}
