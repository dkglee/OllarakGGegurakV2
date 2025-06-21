// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameQuitUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UGameQuitUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* QuitAnim;
	
	UFUNCTION()
	void PlayQuitAnim(bool bIsForward);

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Cancel;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_QuitGame;

	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickQuitGame();
};
