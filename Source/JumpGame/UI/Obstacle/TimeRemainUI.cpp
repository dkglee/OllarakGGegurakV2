// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeRemainUI.h"

#include "Components/RadialSlider.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"

void UTimeRemainUI::NativeConstruct()
{
	Super::NativeConstruct();

	ChangeGaugeValue(0.f);
	TimeText->SetText(FText());
}

void UTimeRemainUI::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(MissionTimerHandle);
	}
	Super::NativeDestruct();
}

void UTimeRemainUI::ChangeGaugeValue(float Value)
{
	MissionTimeRemainSlider->SetValue(Value);
}

void UTimeRemainUI::SetScaleBoxSize(FVector2D Size)
{
	Time_ScaleBox->SetRenderScale(Size);
}

void UTimeRemainUI::SetBarThickness(float Thickness)
{
	MissionTimeRemainSlider->WidgetStyle.BarThickness = Thickness;
}

void UTimeRemainUI::SetTimeTextColor(FLinearColor Color)
{
	TimeText->SetColorAndOpacity(Color);
}

void UTimeRemainUI::TimeTextOff()
{
	TimeText->SetVisibility(ESlateVisibility::Collapsed);
}

void UTimeRemainUI::StartMissionTimer(float InMissionTime)
{
	// 타이머를 원하는 시간
	MissionTime = InMissionTime;
	// 지금 얼마나 시간이 흘렀는지
	CurrentMissionTime = 0;

	// 미션 시작되면 시간을 줄이자
	GetWorld()->GetTimerManager().SetTimer(MissionTimerHandle, this,
		&UTimeRemainUI::MissionTimerTick, GetWorld()->GetDeltaSeconds(), true);
}

void UTimeRemainUI::StopMissionTimer()
{
	// 타이머 끝
	GetWorld()->GetTimerManager().ClearTimer(MissionTimerHandle);
}

void UTimeRemainUI::MissionTimerTick()
{
	CurrentMissionTime += GetWorld()->GetDeltaSeconds();
	
	SetTimeTextTime(static_cast<int>(MissionTime - CurrentMissionTime));
	
	ChangeGaugeValue(CurrentMissionTime / MissionTime);
	if (CurrentMissionTime >= MissionTime)
	{
		StopMissionTimer();
		// 미션 종료 알람을 보내자
		OnMissionTimerEnd.Broadcast();
	}
}

void UTimeRemainUI::SetTimeTextTime(int Time)
{
	TimeText->SetText(FText::AsNumber(Time));

	// if (Time < MissionTime * 0.4)
	// {
	// 	SetTimeTextColor(FLinearColor::Yellow);
	// }
}
