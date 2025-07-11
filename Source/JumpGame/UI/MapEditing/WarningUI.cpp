// Fill out your copyright notice in the Description page of Project Settings.


#include "WarningUI.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

// 1. 충돌 중인 블록이 있는 경우
// 2. 필수 블록이 없는 경우
// 3. 필수 블록이 더 많이 설치된 경우

void UWarningUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WarningBox->SetVisibility(ESlateVisibility::Collapsed);
	WarningToggle->SetVisibility(ESlateVisibility::Collapsed);

	WarningToggle->OnReleased.AddDynamic(this, &UWarningUI::OnWarningButtonReleased);
	WarningToggle->OnHovered.AddDynamic(this, &UWarningUI::OnWarningButtonHovered);
	WarningToggle->OnUnhovered.AddDynamic(this, &UWarningUI::OnWarningButtonUnhovered);
}

void UWarningUI::PlayWarningAnim()
{
	WarningBox->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(WarningForwardAnim);
}

void UWarningUI::SetWarningText(const FString& Text)
{
	WarningText->SetText(FText::FromString(Text));
}

void UWarningUI::SetSubText(const FString& Text)
{
	SubText->SetText(FText::FromString(Text));
}

void UWarningUI::StopWarningAnim()
{
	UUMGSequencePlayer* SequencePlayer = PlayAnimation(WarningReverseAnim);
	if (SequencePlayer)
	{
		auto& FinishedEvent = SequencePlayer->OnSequenceFinishedPlaying();
		TWeakObjectPtr<UWarningUI> WeakThis = this;
		FinishedEvent.AddLambda([WeakThis](UUMGSequencePlayer& Player) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			UWarningUI* StrongThis = WeakThis.Get();
			StrongThis->WarningBox->SetVisibility(ESlateVisibility::Collapsed);
		});
	}
}

void UWarningUI::OnWarningButtonReleased()
{
	bIsButtonClicked = !bIsButtonClicked;

	if (bIsButtonClicked)
	{
		PlayWarningAnim();
	}
	else
	{
		StopWarningAnim();
	}
}

void UWarningUI::WarningOccured()
{
	WarningToggle->SetVisibility(ESlateVisibility::Visible);
	
	if (!bIsWarningOccured)
	{
		PlayAnimation(WarningOccuredAnim);
		PlayAnimation(WarningButtonActivateAnim, 0.f, 0);
	}
	else 
	{
		bIsWarningOccured = true;
	}
}

void UWarningUI::WarningCleared()
{
	UUMGSequencePlayer* SequencePlayer = PlayAnimation(WarningClearedAnim);
	if (SequencePlayer)
	{
		auto& FinishedEvent = SequencePlayer->OnSequenceFinishedPlaying();
		TWeakObjectPtr<UWarningUI> WeakThis = this;
		FinishedEvent.AddLambda([WeakThis](UUMGSequencePlayer& Player) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			UWarningUI* StrongThis = WeakThis.Get();
			StrongThis->WarningToggle->SetVisibility(ESlateVisibility::Collapsed);
		});
	}

	if (bIsButtonClicked)
	{
		StopWarningAnim();
		bIsButtonClicked = false;
	}

	bIsWarningOccured = false;
}

void UWarningUI::OnWarningButtonHovered()
{
	StopAnimation(WarningButtonActivateAnim);
}

void UWarningUI::OnWarningButtonUnhovered()
{
	// 클릭 상태가 아닐 때만 애니메이션을 재생
	if (!bIsButtonClicked)
	{
		PlayAnimation(WarningButtonActivateAnim, 0.f, 0);
	}
}
