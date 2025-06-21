// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PropDragVisual.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UPropDragVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UImage* PropImage;
};
