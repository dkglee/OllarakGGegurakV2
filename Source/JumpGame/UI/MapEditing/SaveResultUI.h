// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveResultUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API USaveResultUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SaveResultAnim;
	UPROPERTY(meta = (BindWidget))
	class UImage* SaveResultImg;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SaveResultText;

	UFUNCTION()
	void PlayResultAnim();
	UFUNCTION()
	void SetResultImage(bool IsSave);
	UFUNCTION()
	void SetResultText(const FString& ResultTest);

	UPROPERTY(EditDefaultsOnly, Category="Status")
	UTexture2D* SaveSuccess;
	UPROPERTY(EditDefaultsOnly, Category="Status")
	UTexture2D* SaveFail;
};
