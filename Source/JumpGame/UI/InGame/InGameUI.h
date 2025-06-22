// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateStarCount(int32 Count);
};
