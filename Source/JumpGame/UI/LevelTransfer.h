// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelTransfer.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ULevelTransfer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Circle;

	UFUNCTION(BlueprintCallable)
	void FillViewport();
};
