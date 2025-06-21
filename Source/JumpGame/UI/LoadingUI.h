// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ULoadingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void InitializeLoadingScreen();
	UFUNCTION()
	void UpdateLoadingImage();
	UFUNCTION()
	void UpdateLoadingProgress(float Value);
	UFUNCTION()
	void UpdateLoadingText();
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* LoadingImage;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* LoadingProgress;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LoadingText;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UTexture2D*> LoadingImageList;
	UPROPERTY(EditDefaultsOnly)
	TArray<FText> LoadingTextList;

	float PrevValue{};
	
	FTimerHandle TimerHandle;
};
