// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditUI.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"

class ALobbyFrog;

void UCreditUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Back->OnClicked.AddDynamic(this, &UCreditUI::UCreditUI::OnClickBack);
	
	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(),ALobbyFrog::StaticClass()));
	CameraComp = Cast<ULobbyCameraComp>(Frog->CameraComp);
}

void UCreditUI::OnClickBack()
{
	// 로비로 돌아가자
	CameraComp->SetViewTarget();
	OnClickBackToLobbyFromCredit.Broadcast();
	RemoveFromParent();
}
