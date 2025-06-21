// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "JumpGame/Core/PlayerController/NetworkPlayerController.h"
#include "NetworkGameState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ANetworkGameState : public AGameState
{
	GENERATED_BODY()

public:
	ANetworkGameState();

	virtual void BeginPlay() override;
	UFUNCTION()
	void VerifyConnection(const FString& NetID);
	UFUNCTION()
	void HandleConnection(const FString& NetID);
		
	UFUNCTION()
	virtual void OnClientAdded(const FString& NetID);
	UFUNCTION()
	virtual void OnAllClientAdded();
	UFUNCTION()
	virtual void OnConnectionSucceeded();
	UFUNCTION()
	virtual void OnConnectionBlocked();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ConnectionSucceeded(const FString& NetID);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RetryConnections(const TArray<FString>& NetIDs);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IO")
	class UIOManagerComponent* IOManagerComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HTTP")
	class UHttpManagerComponent* HttpManagerComponent = nullptr;
	
protected:
	UPROPERTY()
	class UConnectionVerifyComponent* ConnectionVerifyComponent = nullptr;

private:
	
#pragma region NetworkClock
public:
	// 서버 시계를 가져온다.
	virtual double GetServerWorldTimeSeconds() const override;
	
private:
	// 서버의 시간 (앞으로 모든 클라이언트들이 해당 시간을 동기화 할 거임)
	UPROPERTY()
	double ServerWorldTimeSeconds = 0.0f;
public:
	
#pragma endregion

	UFUNCTION()
	void TryConnectToServer();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Connection")
	FTimerHandle ConnectionTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Connection")
	float CheckInterval = 0.25f;
};
