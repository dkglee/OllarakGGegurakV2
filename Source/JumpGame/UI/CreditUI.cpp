// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditUI.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"
#include "UICam/LobbyCameraComp.h"
#include "HAL/PlatformProcess.h"

class ALobbyFrog;

void UCreditUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Back->OnClicked.AddDynamic(this, &UCreditUI::UCreditUI::OnClickBack);
	Btn_Link->OnClicked.AddDynamic(this, &UCreditUI::UCreditUI::OnClickLink);
	
	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(),ALobbyFrog::StaticClass()));
	CameraComp = Cast<ULobbyCameraComp>(Frog->CameraComp);
}

void UCreditUI::OnClickBack()
{
	// 로비로 돌아가자
	CameraComp->SetViewTarget(ECameraState::Main);
	OnClickBackToLobbyFromCredit.Broadcast();
	RemoveFromParent();
}

void UCreditUI::OnClickLink()
{
	const FString URL = TEXT("https://season-bee-387.notion.site/jump-frog");
	FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
}
