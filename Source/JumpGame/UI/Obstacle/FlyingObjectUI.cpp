// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingObjectUI.h"

#include "TimeRemainUI.h"
#include "Components/Image.h"
#include "JumpGame/Utils/FastLogger.h"


void UFlyingObjectUI::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeParameters();
}

void UFlyingObjectUI::InitializeParameters()
{
	TargetLocationImage->SetRenderOpacity(1.f);
	SuccessImage->SetRenderOpacity(0.f);
	FailImage->SetRenderOpacity(0.f);
	OverlapImage->SetRenderOpacity(0.f);
	GaugeImage->SetRenderOpacity(1.f);
	WBP_TimeRemain->SetRenderOpacity(1.f);
	WBP_TimeRemain->SetScaleBoxSize(FVector2D({0.25, 0.25}));
	WBP_TimeRemain->SetBarThickness(25.f);
	WBP_TimeRemain->TimeTextOff();
	
	GaugeValue = 0.f;
}

void UFlyingObjectUI::ChangeGaugeValue(float Value)
{
	Value = FMath::Clamp(Value, 0.0f, 1.0f);
	
	GaugeValue = Value;
	
	GaugeImage->GetDynamicMaterial()->SetScalarParameterValue(FName(TEXT("Gauge")), GaugeValue);
}

float UFlyingObjectUI::GetGaugeValue()
{
	return GaugeValue;
}

void UFlyingObjectUI::SuccessMission()
{
	TargetLocationImage->SetRenderOpacity(0.f);
	SuccessImage->SetRenderOpacity(1.f);
	FailImage->SetRenderOpacity(0.f);
	GaugeImage->SetRenderOpacity(0.f);
	OverlapImage->SetRenderOpacity(0.f);
	WBP_TimeRemain->SetRenderOpacity(0.f);
}

void UFlyingObjectUI::FailMission()
{
	TargetLocationImage->SetRenderOpacity(0.f);
	SuccessImage->SetRenderOpacity(0.f);
	FailImage->SetRenderOpacity(1.f);
	GaugeImage->SetRenderOpacity(0.f);
	OverlapImage->SetRenderOpacity(0.f);
}

void UFlyingObjectUI::Overlapping()
{
	TargetLocationImage->SetRenderOpacity(0.f);
	SuccessImage->SetRenderOpacity(0.f);
	FailImage->SetRenderOpacity(0.f);
	GaugeImage->SetRenderOpacity(1.f);
	OverlapImage->SetRenderOpacity(1.f);
	WBP_TimeRemain->SetRenderOpacity(1.f);
}

void UFlyingObjectUI::NotOverlapping()
{
	TargetLocationImage->SetRenderOpacity(1.f);
	SuccessImage->SetRenderOpacity(0.f);
	FailImage->SetRenderOpacity(0.f);
	GaugeImage->SetRenderOpacity(1.f);
	OverlapImage->SetRenderOpacity(0.f);
	WBP_TimeRemain->SetRenderOpacity(1.f);
}
