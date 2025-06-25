// Fill out your copyright notice in the Description page of Project Settings.


#include "OutToMainUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

void UOutToMainUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OutToMainButton->OnClicked.AddDynamic(this, &UOutToMainUI::OnClickOutToMain);
	ReturnToGameButton->OnClicked.AddDynamic(this, &UOutToMainUI::OnClickReturnToGame);
}

void UOutToMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	AMapGameState* GS{Cast<AMapGameState>(GetWorld()->GetGameState())};
	APlayerController* PC{UGameplayStatics::GetPlayerController(GetWorld(), 0)};
	if (!GS || !PC)
	{
		return;
	}
	
	LeftStarText->SetText(FText::AsNumber(GS->MaxStarCount - GS->StarCount));

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UOutToMainUI::OnClickOutToMain()
{
	// Todo: 나중에 멀티 고려하면 server travel로 ?

	FString CurrentLevelName{UGameplayStatics::GetCurrentLevelName(GetWorld(), true)};
	if (CurrentLevelName == TEXT("InGameLevel"))
	{
		AMapGameState* GS{Cast<AMapGameState>(GetWorld()->GetGameState())};
		if (GS)
		{
			GS->EndStage(false);
		}
		
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/ClientRoomLevel"));
	}
	else
	{
		FLog::Log("Not InGameLevel, Can not Go");
	}
}

void UOutToMainUI::OnClickReturnToGame()
{
	APlayerController* PC{UGameplayStatics::GetPlayerController(GetWorld(), 0)};
	if (!PC)
	{
		return;
	}

	AFrog* Frog{Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn())};
	if (Frog)
	{
		Frog->ResumeFrogMovement();
	}
	
	PC->SetInputMode(FInputModeGameOnly());
	PC->bShowMouseCursor = false;

	CloseUI();	
}

void UOutToMainUI::CloseUI()
{
	RemoveFromParent();
}
