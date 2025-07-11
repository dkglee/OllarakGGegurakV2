// Fill out your copyright notice in the Description page of Project Settings.


#include "Ending.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UEnding::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_GoLobby->OnClicked.AddDynamic(this, &UEnding::UEnding::OnClickedGoLobby);
}

void UEnding::OnClickedGoLobby()
{
	this->RemoveFromParent();
}
