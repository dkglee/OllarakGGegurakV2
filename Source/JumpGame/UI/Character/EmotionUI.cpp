// Fill out your copyright notice in the Description page of Project Settings.


#include "EmotionUI.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "JumpGame/Utils/FastLogger.h"

void UEmotionUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EmotionImages = { Image_Emotion0, Image_Emotion1, Image_Emotion2, Image_Emotion3 };
}

void UEmotionUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	FVector2D Center = ViewportSize / 2.f;
	
	if (bIsInViewport)
	{
		FVector2D MousePos;
		if (GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y))
		{
			UpdateEmotionSelect(MousePos, Center);
		}
	}
}

void UEmotionUI::UpdateEmotionHighlight()
{
	for (int32 i = 0; i < EmotionImages.Num(); i++)
	{
		if (!EmotionImages[i]) continue;

		if (i == CurrentEmotionIndex)
		{
			EmotionImages[i]->SetRenderOpacity(0.5f);
		}
		else
		{
			EmotionImages[i]->SetRenderOpacity(0.f);
		}
	}
}

void UEmotionUI::PlayShowAnim(bool bIsForward)
{
	if (!ShowEmotionUI) return;

	if (bIsForward)
	{
		// 앞으로 재생
		PlayAnimation(ShowEmotionUI);
		bIsInViewport = true;
	}
	else
	{
		// 역재생
		PlayAnimation(ShowEmotionUI, 0, 1, EUMGSequencePlayMode::Reverse);
		bIsInViewport = false;
	}
}

void UEmotionUI::UpdateEmotionSelect(const FVector2D& MousePos, const FVector2D& Center)
{
	FVector2D Dir = MousePos - Center;

	if (Dir.IsNearlyZero())
	{
		CurrentEmotionIndex = -1;
		return;
	}

	Dir.Normalize();
	float AngleRad = FMath::Atan2(Dir.Y, Dir.X); // -π ~ π
	float AngleDeg = FMath::RadiansToDegrees(AngleRad); // -180 ~ 180

	AngleDeg -= 45.f; // 45도 기울이기
	if (AngleDeg < 0.f)
		AngleDeg += 360.f;

	// 인덱스 → 방향 설명
	// 4칸기준: 왼쪽 위부터 0번, 시계방향 회전
	
	float EmotionAngle = 360 / EmotionCount;
	CurrentEmotionIndex = (FMath::FloorToInt(AngleDeg / EmotionAngle) + 2) % EmotionCount;
	UE_LOG(LogTemp, Warning, TEXT("AngleDeg: %f | Index: %d"), AngleDeg, CurrentEmotionIndex);

	UpdateEmotionHighlight();
}

void UEmotionUI::ConfirmEmotionSelection()
{
	if (CurrentEmotionIndex != -1)
	{
		OnEmotionSelected.ExecuteIfBound(CurrentEmotionIndex);
	}
}
