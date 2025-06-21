// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendsList.h"

#include "FriendsItemWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"

class ALobbyPlayerController;

void UFriendsList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->OnFriendListUpdated.BindUObject(this, &UFriendsList::OnFindComplete);
	}

	Btn_Refresh->OnClicked.AddDynamic(this, &UFriendsList::OnClickRefresh);
	// 초기화
	Init();

	// PlayerController에 델리게이트 바인딩
	// auto PC = GetOwningPlayer<ALobbyPlayerController>();
	// if (GetWorld()->GetMapName().Contains(TEXT("Client")) && PC)
	// {
	// 	PC->OnFriendListUpdated.AddDynamic(this, &UFriendsList::OnFriendListReceived);
	//
	// 	// 자동 요청
	// 	PC->RequestFriendList();
	// 	Btn_Refresh->SetIsEnabled(false);
	// }
}


void UFriendsList::Init()
{
	ScrollBox_Friend->ClearChildren();
	FriendsItemPool.Empty();

	for (int32 i = 0; i < MaxFriendCount; i++)
	{
		// Friend Widget 생성
		UFriendsItemWidget* FriendsItemWidget = CreateWidget<UFriendsItemWidget>(this, FriendsItemWidgetFactory);
		FriendsItemWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		// 배열에 저장
		FriendsItemPool.Add(FriendsItemWidget);

		// 스크롤 박스에 추가
		ScrollBox_Friend->AddChild(FriendsItemWidget);
	}
}

void UFriendsList::OnFriendListReceived(const TArray<FSteamFriendData>& FriendList)
{
	AllFoundFriends = FriendList;
	UpdateFriendList(FriendList);
	Btn_Refresh->SetIsEnabled(true);
}

void UFriendsList::UpdateFriendList(const TArray<FSteamFriendData>& FriendDataArray)
{
	UE_LOG(LogTemp, Display, TEXT("FriendsList::UpdateFriendList"));
	for (int i = 0; i < FriendsItemPool.Num(); ++i)
	{
		if (i < FriendDataArray.Num())
		{
			FriendsItemPool[i]->SetFriendInfo(FriendDataArray[i]); // 데이터 바인딩
			FriendsItemPool[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			FriendsItemPool[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UFriendsList::OnClickRefresh()
{
	UE_LOG(LogTemp, Display, TEXT("FriendsList::OnClickRefresh"));
	
	/*AllFoundFriends.Empty(); // 중복 방지
	UpdateFriendList(AllFoundFriends);
	GI->GetSteamFriends();

	// 찾는 중 버튼 비활성화
	Btn_Refresh->SetIsEnabled(false);*/

	if (auto PC = GetOwningPlayer<ALobbyPlayerController>())
	{
		PC->RequestFriendList();
		Btn_Refresh->SetIsEnabled(false);
	}
}

void UFriendsList::OnFindComplete(const FSteamFriendData& Data)
{
	if (Data.FriendIdx == -1) // 종료 신호
	{
		// 활성화
		Btn_Refresh->SetIsEnabled(true);
		UpdateFriendList(AllFoundFriends);
	}
	else
	{
		AllFoundFriends.Add(Data);
	}
}

void UFriendsList::PlayShowAnim(bool bIsForward)
{
	if (!FriendListAnim) return;

	if (bIsForward)
	{
		// 앞으로 재생
		PlayAnimation(FriendListAnim);
		bIsInViewport = true;
	}
	else
	{
		// 역재생
		PlayAnimation(FriendListAnim, 0, 1, EUMGSequencePlayMode::Reverse);
		bIsInViewport = false;
	}
}
