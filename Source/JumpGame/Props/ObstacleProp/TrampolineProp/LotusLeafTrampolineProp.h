// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Props/ObstacleProp/TrampolineProp.h"
#include "LotusLeafTrampolineProp.generated.h"

UCLASS()
class JUMPGAME_API ALotusLeafTrampolineProp : public ATrampolineProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALotusLeafTrampolineProp();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
