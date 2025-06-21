// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EmotionUI.generated.h"

// 선택 결과를 전달하는 델리게이트
DECLARE_DELEGATE_OneParam(FOnEmotionSelected, int32)

UCLASS()
class JUMPGAME_API UEmotionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// UI 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowEmotionUI;
	UFUNCTION()
	void PlayShowAnim(bool bIsForward);

	// 화면에 있나요?
	UPROPERTY()
	bool bIsInViewport = false;

	// 버튼 하이라이팅
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Emotion0;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Emotion1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Emotion2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Emotion3;

	// 이미지 배열
	UPROPERTY()
	TArray<class UImage*> EmotionImages;

	// 버튼 하이라이팅
	UFUNCTION()
	void UpdateEmotionHighlight();
	
	// 버튼 인덱스 선택 관련
	FOnEmotionSelected OnEmotionSelected;

	// 현재 선택된 감정표현 인덱스
	UPROPERTY()
	int32 CurrentEmotionIndex = -1;
	// 감정표현 총 개수
	UPROPERTY()
	int32 EmotionCount = 4;
	
	// 마우스 움직임 따라 선택된 인덱스 업데이트 하는 함수
	UFUNCTION()
	void UpdateEmotionSelect(const FVector2D& MousePos, const FVector2D& Center);
	UFUNCTION()
	void ConfirmEmotionSelection();
};
