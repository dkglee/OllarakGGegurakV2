// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StoryUI.h"
#include "PondFly.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UPondFly : public UStoryUI
{
	GENERATED_BODY()
	
public:
	virtual void InitializeStory() override;
};
