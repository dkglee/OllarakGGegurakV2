// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartSoundUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UStartSoundUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* RecordMovement;

	UFUNCTION()
	void PlayStartSoundAnim();
};
