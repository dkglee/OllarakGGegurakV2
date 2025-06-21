// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeRemainUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerEndDelegate);

UCLASS()
class JUMPGAME_API UTimeRemainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void ChangeGaugeValue(float Value);
	
	void SetScaleBoxSize(FVector2D Size);
	void SetBarThickness(float Thickness);
	void SetTimeTextColor(FLinearColor Color);
	
	void SetTimeTextTime(int Time);
	void TimeTextOff();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URadialSlider* MissionTimeRemainSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* Time_ScaleBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TimeText;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnTimerEndDelegate OnMissionTimerEnd;

	void StartMissionTimer(float InMissionTime);
	void StopMissionTimer();

private:
	void MissionTimerTick();

	FTimerHandle MissionTimerHandle;
	float CurrentMissionTime = 0;
	float MissionTime = 0;
};
