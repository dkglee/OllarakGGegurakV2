// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "LobbyStageCamera.generated.h"

UCLASS()
class JUMPGAME_API ALobbyStageCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALobbyStageCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class ALobbyFrog* TargetFrog = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadZoneHalfWidth = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadZoneHalfHeight = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FollowSpeed = 1.f;
};
