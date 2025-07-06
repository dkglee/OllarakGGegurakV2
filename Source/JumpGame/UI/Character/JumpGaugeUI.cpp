// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpGaugeUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"

void UJumpGaugeUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Frog = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//
	// DelegateBind(Frog);
}

void UJumpGaugeUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UJumpGaugeUI::OnSuperJumpRatioChanged(float NewRatio)
{
	//JumpGaugeSlider->SetPercent(NewRatio);

	if (NewRatio >= 1.f)
	{
		SuperJumpText->SetRenderOpacity(1.f);
		SmallJumpText->SetRenderOpacity(0.f);
		//JumpGaugeSlider->SetFillColorAndOpacity(FLinearColor::Red);
	}
	else if (NewRatio >= 0.5f)
	{
		SuperJumpText->SetRenderOpacity(0.f);
		SmallJumpText->SetRenderOpacity(1.f);
		//JumpGaugeSlider->SetFillColorAndOpacity(FLinearColor::Yellow);
	}
	else
	{
		SuperJumpText->SetRenderOpacity(0.f);
		SmallJumpText->SetRenderOpacity(0.f);
		//JumpGaugeSlider->SetFillColorAndOpacity(FLinearColor::Green);
	}
}

void UJumpGaugeUI::DelegateBind(AFrog* NewFrog)
{
	if (NewFrog)
	{
		NewFrog->OnSuperJumpRatioChanged.RemoveDynamic(this, &UJumpGaugeUI::OnSuperJumpRatioChanged);
		NewFrog->OnSuperJumpRatioChanged.AddDynamic(this, &UJumpGaugeUI::OnSuperJumpRatioChanged);
	}
}
