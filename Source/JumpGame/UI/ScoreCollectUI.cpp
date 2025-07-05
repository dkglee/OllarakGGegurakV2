// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreCollectUI.h"
#include "Components/TextBlock.h"

void UScoreCollectUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurrentScoreText->SetText(FText::FromString(TEXT("0")));
	TotalScoreText->SetText(FText::FromString(TEXT("0")));
}

void UScoreCollectUI::UpdateCurrentScore(int32 CurrentScore)
{
	if (CurrentScoreText)
	{
		CurrentScoreText->SetText(FText::AsNumber(CurrentScore));
	}
}

void UScoreCollectUI::UpdateTotalScore(int32 TotalScore)
{
	if (TotalScoreText)
	{
		TotalScoreText->SetText(FText::AsNumber(TotalScore));
	}
}