// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ending.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UEnding : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoLobby;
	UFUNCTION()
	void OnClickedGoLobby();
};
