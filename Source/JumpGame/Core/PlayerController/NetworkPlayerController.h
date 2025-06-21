// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetworkPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ANetworkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANetworkPlayerController();

	virtual void BeginPlay() override;


	
	
// 네트워크 시계
#pragma region NetworkClock
public:
	// 서버 시계를 가져온다.
	float GetServerTime() const;
	
protected:
	// 네트워크 시계를 만들기 위해서 필요한 함수
	// 아래 함수에서 서버로부터 시간을 받아오도록 요청한다.
	virtual void ReceivedPlayer() override;

private:
	UPROPERTY()
	class UNetworkClockComponent* NetworkClockComponent = nullptr;
#pragma endregion
};
