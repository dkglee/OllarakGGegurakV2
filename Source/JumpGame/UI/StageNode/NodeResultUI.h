// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NodeResultUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UNodeResultUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Star3;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* StarBlackTex;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* StarYellowTex;
	
	UFUNCTION()
	void UpdateNodeStarImages(int32 StarCount);
};
