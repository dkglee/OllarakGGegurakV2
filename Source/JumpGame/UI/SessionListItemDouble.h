// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionListItemDouble.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API USessionListItemDouble : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(class USessionListItemWidget* ListLeft, class USessionListItemWidget* ListRight);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USessionListItemWidget* SessionListLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USessionListItemWidget* SessionListRight;

private:
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* RoomContainer;
};
