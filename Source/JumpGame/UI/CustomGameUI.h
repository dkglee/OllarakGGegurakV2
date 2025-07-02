// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomGameUI.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickBackToLobby);

UCLASS()
class JUMPGAME_API UCustomGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;
	UFUNCTION()
	void OnClickBtn_Back();

	// 카메라 전환하기
	UPROPERTY()
	class ULobbyCameraComp* CameraComp;
	FOnClickBackToLobby OnClickBackToLobby;
};
