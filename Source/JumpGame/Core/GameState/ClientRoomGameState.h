// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "ClientRoomGameState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AClientRoomGameState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	AClientRoomGameState();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConnectionSucceeded() override;
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFollower> FollowerUIClass;
	UPROPERTY(EditAnywhere)
	class UFollower* FollowerUI{nullptr};

	UPROPERTY(EditAnywhere)
	FVector2D PrevLoc;

	FVector2D PrevMousePos{FVector2D::ZeroVector};
	float AngleVelocity{};
	
};
