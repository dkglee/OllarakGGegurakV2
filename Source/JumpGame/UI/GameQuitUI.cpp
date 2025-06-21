// Fill out your copyright notice in the Description page of Project Settings.


#include "GameQuitUI.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameQuitUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Cancel->OnClicked.AddDynamic(this, &UGameQuitUI::OnClickCancel);
	Btn_QuitGame->OnClicked.AddDynamic(this, &UGameQuitUI::OnClickQuitGame);
}

void UGameQuitUI::PlayQuitAnim(bool bIsForward)
{
	if (bIsForward)
	{
		PlayAnimationForward(QuitAnim);
	}
	else
	{
		PlayAnimationReverse(QuitAnim);
	}
}

void UGameQuitUI::OnClickCancel()
{
	PlayQuitAnim(false);
}

void UGameQuitUI::OnClickQuitGame()
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		if (GEditor->IsPlayingSessionInEditor())
		{
			UE_LOG(LogTemp, Warning, TEXT("PIE 상태에서는 종료하지 않음"));
			return;
		}
	}
#endif

	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
	}
	
	Btn_Cancel->SetIsEnabled(false);
	Btn_QuitGame->SetIsEnabled(false);
}
