// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StoryUI.h"
#include "MoonPondUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UMoonPondUI : public UStoryUI
{
	GENERATED_BODY()

public:
	virtual void InitializeStory() override;
};
