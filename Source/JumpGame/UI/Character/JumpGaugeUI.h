// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGaugeUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UJumpGaugeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	class AFrog* Frog;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* JumpGaugeSlider;

	UFUNCTION()
	void OnSuperJumpRatioChanged(float NewRatio);

	UFUNCTION()
	void DelegateBind(AFrog* NewFrog);
};
