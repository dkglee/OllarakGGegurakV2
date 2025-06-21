// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "FriendsItemWidget.generated.h"

class UJumpGameInstance;
/**
 * 
 */
UCLASS()
class JUMPGAME_API UFriendsItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_FriendPicture;
	// 이미지
	UPROPERTY()
	class UTexture2D* Img_Online;
	UPROPERTY()
	class UTexture2D* Img_Offline;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_FriendName;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Invite;

	UPROPERTY()
	UJumpGameInstance* GI;

	UPROPERTY()
	int32 FriendIndex;
	
	UFUNCTION()
	void OnClickInvite();
	void SetFriendInfo(const FSteamFriendData& FriendData);
};
