// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutToMainUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UOutToMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
 	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* OutToMainButton;
 	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ReturnToGameButton;
};
