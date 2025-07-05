// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreCollectUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UScoreCollectUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void UpdateCurrentScore(int32 CurrentScore);
	UFUNCTION()
	void UpdateTotalScore(int32 TotalScore);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentScoreText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalScoreText;
};
