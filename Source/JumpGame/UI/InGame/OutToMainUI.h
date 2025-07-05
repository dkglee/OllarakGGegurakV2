// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutToMainUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UOutToMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnClickOutToMain();
	UFUNCTION()
	void OnClickReturnToGame();
	UFUNCTION()
	void CloseUI();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LeftStarText;
 	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* OutToMainButton;
 	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ReturnToGameButton;

	// 테스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UClientRoomUI> ClientRoomUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UClientRoomUI* ClientRoomUI;
};
