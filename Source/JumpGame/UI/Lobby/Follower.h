// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Follower.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UFollower : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateLocation();
	void UpdateRotation();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* FollowerImage;
};
