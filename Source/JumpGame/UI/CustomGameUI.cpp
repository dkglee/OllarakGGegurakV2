// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameUI.h"

#include "Components/Button.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"
#include "UICam/LobbyCameraComp.h"

void UCustomGameUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Back->OnClicked.AddDynamic(this, &UCustomGameUI::OnClickBtn_Back);
	
	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(), ALobbyFrog::StaticClass()));
	CameraComp = Cast<ULobbyCameraComp>(Frog->CameraComp);
}

void UCustomGameUI::OnClickBtn_Back()
{
	CameraComp->SetViewTarget(ECameraState::Main);
	OnClickBackToLobby.Broadcast();
	RemoveFromParent();
}
