// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGameMode.h"
#include "InGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AInGameMode : public AMapGameMode
{
	GENERATED_BODY()

public:
	AInGameMode();

protected:
	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UPROPERTY()
	TArray<int32> AvailableSkinIndices;
};
