// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetworkPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ANetworkPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void OnSetUniqueId() override;
	bool ConnectToServer();
	
#pragma region NetworkVerify
public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_HandShake(const FString& NetID);
	UFUNCTION(Server, Reliable)
	void ServerRPC_ConnectionVerified(const FString& NetID);
#pragma endregion
	
	bool bIsSetUniqueId = false;
};
