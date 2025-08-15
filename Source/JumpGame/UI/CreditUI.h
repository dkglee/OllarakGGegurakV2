// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditUI.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickBackToLobbyFromCredit);

UCLASS()
class JUMPGAME_API UCreditUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CreditCanvas;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Link;

	UFUNCTION()
	void OnClickBack();
	UFUNCTION()
	void OnClickLink();

	// 카메라 전환하기
	UPROPERTY()
	class ULobbyCameraComp* CameraComp;

	FOnClickBackToLobbyFromCredit OnClickBackToLobbyFromCredit;
};
