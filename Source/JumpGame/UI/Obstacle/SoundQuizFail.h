// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoundQuizFail.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API USoundQuizFail : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* QuizFailAnim;

	UFUNCTION()
	void PlayQuizFailAnim();
};
