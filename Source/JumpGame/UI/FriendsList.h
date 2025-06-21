// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "FriendsList.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UFriendsList : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY()
	UJumpGameInstance* GI;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Friend;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Refresh;
	
	UPROPERTY(editanywhere, BlueprintReadWrite)
	TSubclassOf<class UFriendsItemWidget> FriendsItemWidgetFactory;

	// 오브젝트 풀
	UPROPERTY(editanywhere)
	TArray<UFriendsItemWidget*> FriendsItemPool;
	// 찾아진 친구 저장
    UPROPERTY(editanywhere)
    TArray<FSteamFriendData> AllFoundFriends;
    // 최대 친구 목록 수
    UPROPERTY(editanywhere)
    int32 MaxFriendCount = 20;

	UFUNCTION()
	void OnFriendListReceived(const TArray<FSteamFriendData>& FriendList);

	UFUNCTION()
	void Init();
	UFUNCTION()
	void UpdateFriendList(const TArray<FSteamFriendData>& FriendDataArray);
	
	UFUNCTION()
	void OnClickRefresh();
	
	// 델리게이트 함수
	void OnFindComplete(const FSteamFriendData& Data);
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FriendListAnim;
	UFUNCTION()
	void PlayShowAnim(bool bIsForward);
	
	UPROPERTY()
	bool bIsInViewport = false;
};

