// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "JumpSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UJumpSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TMap<FName, int32> SavedFieldStarCountMap;

	UPROPERTY()
	TMap<FName, float> SavedFieldClearTimeMap;
};
