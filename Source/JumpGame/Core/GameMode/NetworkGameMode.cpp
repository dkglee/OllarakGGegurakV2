// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameMode.h"

#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Core/PlayerController/NetworkPlayerController.h"
#include "JumpGame/Core/PlayerState/NetworkPlayerState.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"
#include "JumpGame/Utils/FastLogger.h"

ANetworkGameMode::ANetworkGameMode()
{
	bReplicates = true;
	
	PlayerControllerClass = ANetworkPlayerController::StaticClass();
	GameStateClass = ANetworkGameState::StaticClass();
	PlayerStateClass = ANetworkPlayerState::StaticClass();
}

void ANetworkGameMode::BeginPlay()
{
	Super::BeginPlay();
}
