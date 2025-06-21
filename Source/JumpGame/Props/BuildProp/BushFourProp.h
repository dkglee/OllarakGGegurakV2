// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildProp.h"
#include "BushFourProp.generated.h"

UCLASS()
class JUMPGAME_API ABushFourProp : public ABuildProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABushFourProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
