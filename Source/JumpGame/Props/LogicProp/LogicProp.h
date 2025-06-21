// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpGame/Props/BaseProp.h"
#include "LogicProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API ALogicProp : public ABaseProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALogicProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
