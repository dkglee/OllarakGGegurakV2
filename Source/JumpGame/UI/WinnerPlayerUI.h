// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinnerPlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UWinnerPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* WinnerClearAnim;

	UFUNCTION()
	void PlayWinnerAnim();
};
