// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapLoadingUI.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UMapLoadingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LoadingText;
	UFUNCTION()
	void SetLoadingText(const FString& Text);

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* MapLoadingAnim;
	
	// 삭제할때 anim reverse 말고 바로 삭제하길 바람
	// 참고: blur가 있으니 remove 해야함 (hidden 하지마시오)
	UFUNCTION()
	void PlayLoadingAnim(); 
};
