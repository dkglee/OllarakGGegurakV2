// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPlayerController.h"

#include "NetworkClockComponent.h"
#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"

ANetworkPlayerController::ANetworkPlayerController()
{
	bReplicates = true;
	
	NetworkClockComponent = CreateDefaultSubobject<UNetworkClockComponent>("Network Clock Component");
	NetworkClockComponent->SetNetAddressable();
	NetworkClockComponent->SetIsReplicated(true);
}

float ANetworkPlayerController::GetServerTime() const
{
	if (NetworkClockComponent)
	{
		return NetworkClockComponent->GetServerTime();
	}
	return GetWorld()->GetTimeSeconds();
}

// PlayerController가 서버로부터 생성되었을 때 호출되는 함수
// 해당 함수에서 서버 시간을 요청한다.
void ANetworkPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (NetworkClockComponent)
	{
		NetworkClockComponent->RequestServerTime(this);
	}
}
	
void ANetworkPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}
