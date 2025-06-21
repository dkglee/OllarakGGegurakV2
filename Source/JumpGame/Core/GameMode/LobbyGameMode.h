// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameMode.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ALobbyGameMode : public ANetworkGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UPROPERTY()
	int32 PlayerIdx = 0;
	
	UPROPERTY()
	class UJumpGameInstance* GI;
};
