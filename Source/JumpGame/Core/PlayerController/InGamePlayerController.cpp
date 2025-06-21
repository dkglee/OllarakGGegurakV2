// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"

#include "Components/WidgetComponent.h"
#include "JumpGame/Characters/Frog.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	StartInitialize();

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

void AInGamePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	StartInitialize();
}

void AInGamePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	
	StartInitialize();
}

void AInGamePlayerController::StartInitialize()
{
	AFrog* Frog{Cast<AFrog>(GetPawn())};
	if (!Frog)
	{
		return;
	}

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
	
	Frog->InitJumpGaugeUIComponent();

}
