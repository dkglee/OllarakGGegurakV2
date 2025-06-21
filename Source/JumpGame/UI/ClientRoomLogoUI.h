// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "ClientRoomLogoUI.generated.h"


/**
 * 
 */
UCLASS()
class JUMPGAME_API UClientRoomLogoUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UPROPERTY()
	class UJumpGameInstance* GI;
	
	// 로고
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* LogoAnimation;
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_GoMainStart;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Circle;
	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial;

	UFUNCTION()
	void OnClickGoMainStart();
	UFUNCTION()
	void InitLogoMaterial();
	UFUNCTION()
	void PlayLogoAnim(bool bIsForward);

	// 로고 배경
	UPROPERTY()
	float RadiusStartTime = 0.f;
	UPROPERTY()
	bool bIsRadiusAnimating = false;
	UPROPERTY()
	float RadiusDuration = 0.7f; // 몇초동안 원형 변화
	UFUNCTION()
	void RadiusAnimating();
};
