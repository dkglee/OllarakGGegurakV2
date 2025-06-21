// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryPageUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UVictoryPageUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerName;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoLobby;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* WinnerAnim;

	UFUNCTION()
	void OnClickGoLobby();
	UFUNCTION()
	void SetVictoryPlayerName(FString PlayerName);
	UFUNCTION()
	void PlayWinnerPageAnim();

};
