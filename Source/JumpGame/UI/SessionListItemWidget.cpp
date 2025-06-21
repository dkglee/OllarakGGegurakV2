// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"

void USessionListItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Join->OnClicked.AddDynamic(this,&USessionListItemWidget::OnClickJoin);
}

void USessionListItemWidget::OnClickJoin()
{
	// 버튼을 비활성화
	Btn_Join->SetIsEnabled(false);
	
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	GI->JoinOtherSession(RoomIdx);
}

void USessionListItemWidget::SetRoomInfo(const FRoomData& Data)
{
	// 현재 몇번째 방인지?
	RoomIdx = Data.RoomID;
	// 방 정보 받기 (업데이트)
	Text_RoomInfo->SetText(FText::FromString(Data.RoomName));
	Text_MapName->SetText(FText::FromString(Data.MapName));
	Text_PlayerCount->SetText(FText::AsNumber(Data.PlayerCount));
	Text_MaxPlayer->SetText(FText::AsNumber(Data.MaxPlayer));

	// 비밀방인지?
	if (Data.bIsLocked)
	{
		// Img_Secret->SetVisibility(ESlateVisibility::Visible);
	}
}
