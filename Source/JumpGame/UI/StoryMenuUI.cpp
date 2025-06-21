// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryMenuUI.h"

#include "ClientRoomUI.h"
#include "Components/Button.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LotusLeafRiceUI.h"
#include "Lobby/MoonPondUI.h"
#include "Lobby/StoryUI.h"
#include "Lobby/BigBigBee.h"
#include "Lobby/PondFly.h"
#include "Lobby/FrogHill.h"
#include "Lobby/CookedCarrotUI.h"
#include "UICam/LobbyCameraComp.h"

UStoryMenuUI::UStoryMenuUI(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMoonPondUI> MoonPondUI_ClassFinder(
		TEXT("/Game/UI/Story/WBP_MoonPondStory.WBP_MoonPondStory_C")
	);
	if (MoonPondUI_ClassFinder.Succeeded())
	{
		MoonPondUIClass = MoonPondUI_ClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<ULotusLeafRiceUI> LotusLeafRiceUI_ClassFinder(
		TEXT("/Game/UI/Story/WBP_RiceStory.WBP_RiceStory_C")
	);
	if (LotusLeafRiceUI_ClassFinder.Succeeded())
	{
		LotusLeafRiceUIClass = LotusLeafRiceUI_ClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UFrogHill> FrogHillUI_ClassFinder(
		TEXT("/Game/UI/Story/WBP_FrogHillStory.WBP_FrogHillStory_C")
	);
	if (FrogHillUI_ClassFinder.Succeeded())
	{
		FrogHillUIClass = FrogHillUI_ClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UPondFly> PondFlyUI_ClassFinder(
		TEXT("/Game/UI/Story/WBP_PondFlyStory.WBP_PondFlyStory_C")
	);
	if (PondFlyUI_ClassFinder.Succeeded())
	{
		PondFlyUIClass = PondFlyUI_ClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UCookedCarrotUI> CookedCarrotUI_ClassFinder(
		TEXT("/Game/UI/Story/WBP_CookedCarrotStory.WBP_CookedCarrotStory_C")
	);
	if (CookedCarrotUI_ClassFinder.Succeeded())
	{
		CookedCarrotUIClass = CookedCarrotUI_ClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UBigBigBee> BigBigBeeUI_ClassFinder(
		TEXT("/Game/UI/Story/WBP_BigBigBeeStory.WBP_BigBigBeeStory_C")
	);
	if (BigBigBeeUI_ClassFinder.Succeeded())
	{
		BigBigBeeUIClass = BigBigBeeUI_ClassFinder.Class;
	}
}

void UStoryMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Back->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickBtn_Back);
	Btn_Ep1->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp1);
	Btn_Ep2->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp2);
	Btn_Ep3->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp3);
	Btn_Ep4->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp4);
	Btn_Ep5->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp5);
	Btn_Ep6->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp6);

	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(), ALobbyFrog::StaticClass()));
	CameraComp = Cast<ULobbyCameraComp>(Frog->CameraComp);
}

void UStoryMenuUI::OnClickBtn_Back()
{
	// 로비로 돌아가자
	CameraComp->SetViewTarget();
	OnClickBackToLobby.Broadcast();
	RemoveFromParent();
}

void UStoryMenuUI::OnClickMainStory()
{
	//
}

void UStoryMenuUI::OnClickEp1()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && LotusLeafRiceUIClass)
	{
		UStoryUI::CreateAndShowStory<ULotusLeafRiceUI>(OwningPlayer, LotusLeafRiceUIClass);
	}
}

void UStoryMenuUI::OnClickEp2()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && MoonPondUIClass)
	{
		UStoryUI::CreateAndShowStory<UMoonPondUI>(OwningPlayer, MoonPondUIClass);
	}
}

void UStoryMenuUI::OnClickEp3()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && BigBigBeeUIClass)
	{
		UStoryUI::CreateAndShowStory<UBigBigBee>(OwningPlayer, BigBigBeeUIClass);
	}
}

void UStoryMenuUI::OnClickEp4()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && CookedCarrotUIClass)
	{
		UStoryUI::CreateAndShowStory<UCookedCarrotUI>(OwningPlayer, CookedCarrotUIClass);
	}
}

void UStoryMenuUI::OnClickEp5()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && FrogHillUIClass)
	{
		UStoryUI::CreateAndShowStory<UFrogHill>(OwningPlayer, FrogHillUIClass);
	}
}

void UStoryMenuUI::OnClickEp6()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && PondFlyUIClass)
	{
		UStoryUI::CreateAndShowStory<UPondFly>(OwningPlayer, PondFlyUIClass);
	}
}
