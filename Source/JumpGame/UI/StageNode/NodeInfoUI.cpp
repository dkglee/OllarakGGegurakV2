// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeInfoUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UNodeInfoUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_GameStart->OnClicked.AddDynamic(this, &UNodeInfoUI::OnClickGameStart);
}

void UNodeInfoUI::OnClickGameStart()
{
	// 필드 이름받아서 이동
	// UGameplayStatics::OpenLevel(GetWorld(), TEXT(""));
}

void UNodeInfoUI::SetFieldInfo(FName CurrentFieldName, int32 StarCount, float ClearTime)
{
	Text_FieldName->SetText(FText::FromName(CurrentFieldName));
	UpdateStarImages(StarCount);
	UpdateClearTime(ClearTime);
}

void UNodeInfoUI::UpdateStarImages(int32 StarCount)
{
	StarCount = FMath::Clamp(StarCount, 0, 3);
	TArray<UImage*> StarImages = { Image_Star1, Image_Star2, Image_Star3 };

	for (int32 i = 0; i < 3; ++i)
	{
		if (StarImages[i])
		{
			if (i < StarCount)
			{
				// 노란 별 텍스처 세팅
				StarImages[i]->SetBrushFromTexture(StarYellowTex); 
			}
			else
			{
				// 검은 별 텍스처 세팅
				StarImages[i]->SetBrushFromTexture(StarBlackTex);
			}
		}
	}
}

void UNodeInfoUI::UpdateClearTime(float ClearTime)
{
	const int32 TotalSeconds = FMath::RoundToInt(ClearTime);
	const int32 Minutes = TotalSeconds / 60;
	const int32 Seconds = TotalSeconds % 60;

	const FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	if (Text_Time)
	{
		Text_Time->SetText(FText::FromString(TimeText));
	}
}