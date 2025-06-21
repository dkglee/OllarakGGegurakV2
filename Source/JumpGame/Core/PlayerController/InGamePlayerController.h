// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkPlayerController.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AInGamePlayerController : public ANetworkPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;
	virtual void ReceivedPlayer() override;

	void StartInitialize();
};
