// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageResultUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UStageResultUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* OutToMainButton;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ClearAnimation;
};
