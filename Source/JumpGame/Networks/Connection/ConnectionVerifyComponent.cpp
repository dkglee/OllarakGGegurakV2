#include "ConnectionVerifyComponent.h"

#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Utils/FastLogger.h"

// Sets default values for this component's properties
UConnectionVerifyComponent::UConnectionVerifyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	// ...
}

void UConnectionVerifyComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwnerRole() == ROLE_Authority)
	{
		RecursiveCheckClientVerified();
	}
}

void UConnectionVerifyComponent::RecursiveCheckClientVerified()
{
	// 모든 클라이언트가 추가되지 않은 경우
	GetWorld()->GetTimerManager().ClearTimer(ConnectionTimer);
	
	TArray<FString> UnVerifiedClients;
	
	if (CheckAllClientAdded(UnVerifiedClients))
	{
		return ;
	}

	if (UnVerifiedClients.Num() > 0)
	{
		ANetworkGameState* GameState = Cast<ANetworkGameState>(GetWorld()->GetGameState());
		
		GameState->MulticastRPC_RetryConnections(UnVerifiedClients);
	}
	
	GetWorld()->GetTimerManager().SetTimer(ConnectionTimer, this, &UConnectionVerifyComponent::RecursiveCheckClientVerified,
		CheckInterval, false);
}

void UConnectionVerifyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 타이머를 종료한다.
	GetWorld()->GetTimerManager().ClearTimer(ConnectionTimer);
	
	Super::EndPlay(EndPlayReason);
}

void UConnectionVerifyComponent::InitMaxPlayerCount(const int32 InMaxPlayerCount)
{
	// 최대 플레이어 수를 설정한다.
	MaxPlayerCount = InMaxPlayerCount;
}

void UConnectionVerifyComponent::AddClient(const FString& NetID)
{
	if (ClientMap.Contains(NetID))
	{
		FFastLogger::LogConsole(TEXT("Error: Client %s is already added"), *NetID);
		// 이미 추가된 클라이언트인 경우
		return;
	}

	FFastLogger::LogConsole(TEXT("Add Client %s"), *NetID);
	ClientMap.Add(NetID, false);
}

void UConnectionVerifyComponent::ConfirmClient(const FString& NetID)
{
	if (ClientMap.Contains(NetID) && !ClientMap[NetID])
	{
		ClientMap[NetID] = true;
		OnClientAdded.Broadcast(NetID);
	}
	else if (ClientMap.Contains(NetID) && ClientMap[NetID])
	{
		// 이미 추가된 클라이언트인 경우
		FFastLogger::LogConsole(TEXT("Error: Client %s is already added"), *NetID);
	}
	else
	{
		// 클라이언트가 추가되지 않은 경우
		FFastLogger::LogConsole(TEXT("Error: Client %s is not added"), *NetID);
		return;
	}
}

void UConnectionVerifyComponent::SetClientVerify(bool bCond)
{
	bClientVerify = bCond;
	if (bClientVerify)
	{
		OnConnectionSucceeded.Broadcast();
	}
	else
	{
		OnConnectionBlocked.Broadcast();
	}
}


bool UConnectionVerifyComponent::CheckAllClientAdded(TArray<FString>& UnVerifiedClients)
{
	bool bAllClientAdded = true;

	if (MaxPlayerCount < 0 || ClientMap.Num() < MaxPlayerCount)
	{
		// 아직 모든 클라이언트가 추가되지 않은 경우
		return false;
	}
	
	for (const auto& Client : ClientMap)
	{
		if (!Client.Value)
		{
			UnVerifiedClients.Add(Client.Key);
		}
		bAllClientAdded &= Client.Value;
	}

	if (bAllClientAdded)
	{
		OnAllClientAdded.Broadcast();
	}
	
	return bAllClientAdded;
}
