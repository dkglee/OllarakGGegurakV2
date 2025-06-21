// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoadingUI.h"

#include "Components/TextBlock.h"

void UMapLoadingUI::SetLoadingText(const FString& Text)
{
	LoadingText->SetText(FText::FromString(Text));
}

void UMapLoadingUI::PlayLoadingAnim()
{
	PlayAnimation(MapLoadingAnim);
}
