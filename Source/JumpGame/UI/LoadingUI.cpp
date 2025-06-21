// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingUI.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULoadingUI::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	Super::NativeDestruct();
}

void ULoadingUI::InitializeLoadingScreen()
{
	UpdateLoadingImage();
	UpdateLoadingText();
	UpdateLoadingProgress(0.f);
	PrevValue = 0.f;
}

void ULoadingUI::UpdateLoadingProgress(float Value)
{
	// 천천히 증가
	TWeakObjectPtr<ULoadingUI> WeakThis{this};
	FTimerDelegate LoadingDelegate{
		FTimerDelegate::CreateLambda([WeakThis, Value]() {
			if (WeakThis.IsValid())
			{
				ULoadingUI* StrongThis = WeakThis.Get();
				StrongThis->PrevValue += StrongThis->GetWorld()->GetDeltaSeconds();
				
				if (StrongThis->PrevValue > Value)
				{
					StrongThis->LoadingProgress->SetPercent(Value);
					StrongThis->PrevValue = Value;

					StrongThis->GetWorld()->GetTimerManager().ClearTimer(StrongThis->TimerHandle);
					return;
				}
				
				StrongThis->LoadingProgress->SetPercent(StrongThis->PrevValue);
			}
		})
	};
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, LoadingDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void ULoadingUI::UpdateLoadingImage()
{
	int32 RandIndex{FMath::RandRange(0, LoadingImageList.Num() - 1)};
	if (LoadingImageList.IsValidIndex(RandIndex))
	{
		LoadingImage->SetBrushFromTexture(LoadingImageList[RandIndex]);
	}
}

void ULoadingUI::UpdateLoadingText()
{
	int32 RandIndex = FMath::RandRange(0, LoadingTextList.Num() - 1);
	if (LoadingTextList.IsValidIndex(RandIndex))
	{
		LoadingText->SetText(LoadingTextList[RandIndex]);
	}
}
