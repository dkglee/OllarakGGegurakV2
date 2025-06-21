// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlyingObjectUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UFlyingObjectUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void ChangeGaugeValue(float Value);
	void InitializeParameters();
	float GetGaugeValue();
	void SuccessMission();
	void FailMission();
	void Overlapping();
	void NotOverlapping();
	// 애니메이션 재생하고 UI 지움
	UFUNCTION(BlueprintImplementableEvent)
	void VanishMission();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* TargetLocationImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* SuccessImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* FailImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* OverlapImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* GaugeImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTimeRemainUI* WBP_TimeRemain;

public:
	float GaugeValue;
};
