// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendsItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"


void UFriendsItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());

	Btn_Invite->OnClicked.AddDynamic(this, &UFriendsItemWidget::OnClickInvite);

	Img_Online = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/UI/Image/Profile_On.Profile_On")));
	Img_Offline = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/UI/Image/Profile_Off.Profile_Off")));
}

void UFriendsItemWidget::OnClickInvite()
{
	if (GI)
	{
		const auto& Friends = GI->GetFilteredFriendList();
		if (Friends.IsValidIndex(FriendIndex))
		{
			GI->InviteFriendToSession(Friends[FriendIndex].SteamId);
		}
	}
}

void UFriendsItemWidget::SetFriendInfo(const FSteamFriendData& FriendData)
{
	if (Text_FriendName)
	{
		Text_FriendName->SetText(FText::FromString(FriendData.DisplayName));
	}

	// 이미지 업데이트
	const bool bIsOnline = FriendData.bIsOnline;
	Btn_Invite->SetIsEnabled(bIsOnline);
	Img_FriendPicture->SetBrushFromTexture(bIsOnline ? Img_Online : Img_Offline);

	FString SteamId = FriendData.SteamId;
	FriendIndex = FriendData.FriendIdx;
}
