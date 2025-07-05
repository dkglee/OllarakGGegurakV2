// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeInfoUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "JumpGame/UI/LevelTransfer.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

UNodeInfoUI::UNodeInfoUI(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	ConstructorHelpers::FClassFinder<ULevelTransfer> WidgetTransferUIWidget
		(TEXT("/Game/UI/LobbyUI/WBP_LevelTransfer.WBP_LevelTransfer_C"));
	if (WidgetTransferUIWidget.Succeeded())
	{
		WidgetTransferUIClass = WidgetTransferUIWidget.Class;
	}
}

void UNodeInfoUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (WidgetTransferUIClass)
	{
		WidgetTransferUI = CreateWidget<ULevelTransfer>(GetWorld(), WidgetTransferUIClass);
	}
}

void UNodeInfoUI::OnClickGameStart()
{
	WidgetTransferUI->SetVisibility(ESlateVisibility::HitTestInvisible);
	WidgetTransferUI->AddToViewport(999);

	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UNodeInfoUI::TransitionAnimation,
	                                       0.016f, true);
}

void UNodeInfoUI::TransitionAnimation()
{
	RadiusValue += 0.02f * 2.f;

	UMaterialInstanceDynamic* DynamicMaterial{WidgetTransferUI->Image_Circle->GetDynamicMaterial()};
	DynamicMaterial->SetScalarParameterValue("Radius", RadiusValue);

	//FLog::Log("", RadiusValue);
	if (RadiusValue >= 3.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TransitionTimer);
		MoveToField();
	}
}

void UNodeInfoUI::MoveToField()
{
	// 필드 이름받아서 이동
	UStageSystemSubsystem* SGI = GetWorld()->GetGameInstance()->GetSubsystem<UStageSystemSubsystem>();

	SGI->SetChosenField(SelectFieldLevel);
	SGI->bNodeRestore = true;
	FFastLogger::LogConsole(TEXT("SelectedFieldLevel : %s"), *SelectFieldLevel.ToString());
	
	const FStageTableRow* StageInfo = SGI->GetStage(SGI->GetChosenStage());
	if (StageInfo)
	{
		UGameplayStatics::OpenLevel(GetWorld(), StageInfo->StageWorld);
	}
}

void UNodeInfoUI::SetFieldInfo(FText CurrentFieldName, int32 StarCount, float ClearTime)
{
	Text_FieldName->SetText(FText(CurrentFieldName));
	UpdateStarImages(StarCount);
	UpdateClearTime(ClearTime);
	UpdateClearText(StarCount, ClearTime);
}

void UNodeInfoUI::UpdateStarImages(int32 StarCount)
{
	StarCount = FMath::Clamp(StarCount, 0, 3);
	TArray<UImage*> StarImages = {Image_Star1, Image_Star2, Image_Star3};

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

	if (ClearTime == 0)
	{
		if (Text_Time)
		{
			Text_Time->SetText(FText::FromString(TEXT("--:--")));
		}
	}
	else
	{
		if (Text_Time)
		{
			Text_Time->SetText(FText::FromString(TimeText));
		}
	}
}

void UNodeInfoUI::UpdateClearText(int32 StarCount, float ClearTime)
{
	if (StarCount == 0 && ClearTime == 0)
	{
		Text_ClearTF->SetText(FText::FromString(TEXT("도전 기록 없음")));
	}
	else
	{
		if (StarCount == 3)
		{
			Text_ClearTF->SetText(FText::FromString(TEXT("클리어 기록!")));
		}
		else
		{
			Text_ClearTF->SetText(FText::FromString(TEXT("도전 진행 중")));
		}
	}
}
