// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NodeTutorialUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UNodeTutorialUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void PlayTutorialAnim();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* TutorialAnim;

	FTimerHandle TutorialAnimTimer;
};
