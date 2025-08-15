// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutroCinematic.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UOutroCinematic : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	// 미디어 플레이어 (실제 재생기)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Media")
	class UMediaPlayer* MediaPlayer;

	// mp4 등 외부 파일 소스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Media")
	class UFileMediaSource* MediaSource;

	// 재생 속도
	UPROPERTY()
	float CurrentPlayRate = 1.0f;
	
	// 스킵
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* SkipAnim;
	UFUNCTION(BlueprintCallable, Category="Media")
	void PlaySkipAnim();

	FTimerHandle AnimOutTimerhandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Media")
	class UButton* Btn_Skip;
	UFUNCTION()
	void OnClickSkip();
};
