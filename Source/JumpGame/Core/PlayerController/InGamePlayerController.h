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
	AInGamePlayerController();
	
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;
	virtual void ReceivedPlayer() override;

	void StartInitialize();

public:
	// 인게임 UI
	UPROPERTY()
	TSubclassOf<class UInGameUI> InGameUIClass;
	UPROPERTY()
	UInGameUI* InGameUI{nullptr};
	// 결과 UI
	UPROPERTY()
	TSubclassOf<class UStageResultUI> ResultUIClass;
	UPROPERTY()
	UStageResultUI* ResultUI{nullptr};
	UPROPERTY()
	TSubclassOf<class UOutToMainUI> OutToMainUIClass;
	UPROPERTY()
	UOutToMainUI* OutToMainUI{nullptr};

	void ShowInGameUI();
	void ShowOutToMainUI();
	void ShowClearUIAnimation();
	void ShowResultUI();
	void UpdateStarCount(int32 Count);
};
