// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoundQuizClear.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API USoundQuizClear : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* QuizClearAnim;

	UFUNCTION()
	void PlayQuizAnim();
};
