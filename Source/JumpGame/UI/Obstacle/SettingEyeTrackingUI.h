// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingEyeTrackingUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API USettingEyeTrackingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* SettingLocationImage;
};
