// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "DefeatPlatform.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API ADefeatPlatform : public ALogicProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADefeatPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 1등 아닌 유저들 소환할 위치들 반환
	FVector SpawnDefeatCharacter();
};
