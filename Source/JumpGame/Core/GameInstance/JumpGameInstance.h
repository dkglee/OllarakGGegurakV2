// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "JumpGameInstance.generated.h"

// 세션 검색 완료시 호출되는 함수를 등록하는 델리게이트
// DECLARE_DELEGATE_TwoParams(FFindComplete, int32, FString);

DECLARE_DELEGATE_OneParam(FFindComplete, const FRoomData&);
DECLARE_DELEGATE_OneParam(FFriendListUpdated, const FSteamFriendData&);

/**
 * 
 */
UCLASS()
class JUMPGAME_API UJumpGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	// 세션 이름
	UPROPERTY()
	FName CurrentSessionName;
	
	// 세션 생성 관련
	UFUNCTION(BlueprintCallable)
	void CreateMySession(FString DisplayName, int32 PlayerCount, const FString& Password);
	UFUNCTION(BlueprintCallable)
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// 세션 조회 관련
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	UFUNCTION(BlueprintCallable)
	void OnFindSessionComplete(bool bWasSuccessful);

	// 세션 참여 관련
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 SessionIdx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// 세션 파괴 관련
	UFUNCTION(BlueprintCallable)
	void LeaveSession(bool bDestroySession);
	UFUNCTION(BlueprintCallable)
	void OnDestroySessionComplete(FName Name, bool bArg);

	void OnFailureSessionDetected(const FUniqueNetId& UniqueNetId, ESessionFailure::Type Arg);
	void OnEndSessionComplete(FName Name, bool bArg);
	
	// 스팀 세션 제목을 한글로 했을 때 깨짐 현상
	FString StringBase64Encode(FString Str);
	FString StringBase64Decode(FString Str);

public:
	// 세션의 모든 처리를 진행해주는 객체
	IOnlineSessionPtr SessionInterface;

	// 세션 검색할 때 사용하는 객체
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// 세션 검색완료시 호출되는 델리게이트
	FFindComplete OnFindComplete;

public:
	// 친구 검색 관련
	UPROPERTY()
	int32 LocalUserNumber = 0;
	
	void GetSteamFriends();
	void OnReadFriendsComplete(int32 LocalPlayer, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);
	void InviteFriendToSession(const FString& FriendIdStr);
	void OnSessionInviteAccepted(bool bWasSuccessful, int32 LocalUserNum,
	TSharedPtr<const FUniqueNetId> InvitingPlayerId, const FOnlineSessionSearchResult& SessionToJoin);

	// 친구리스트 갱신 시 브로드캐스트 될 델리게이트
	FFriendListUpdated OnFriendListUpdated;
	
private:
	// FString NetID;
	TMap<FString, FPlayerInfo> PlayerMap;
	TArray<FSteamFriendData> FilteredFriendList;

public:
	void SetPlayerInfo(const TMap<FString, FPlayerInfo> info) { PlayerMap = info; }
	void AddPlayerInfo(const FString& PlayerKey, const FPlayerInfo& PlayerInfo) { PlayerMap.Add(PlayerKey, PlayerInfo); }
	TMap<FString, FPlayerInfo>& GetPlayerInfo() { return PlayerMap; }

	void SetMapFilePath(const FString& Path) { MapFilePath = Path; }
	FString GetMapFilePath() { return MapFilePath; }
	void ClearMapFilePath() { MapFilePath = FString(); }

	void SetFilteredFriendList(const TArray<FSteamFriendData>& FriendList);
	TArray<FSteamFriendData>& GetFilteredFriendList() { return FilteredFriendList; }

private:
	UPROPERTY()
	int32 MaxPlayerCount = 0;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetMaxPlayerCount() { return MaxPlayerCount; }
	// 게임 시작 버튼을 누를때 설정해주자
	void SetMaxPlayer(int32 MaxPlayer) { MaxPlayerCount = MaxPlayer; };
	
public:
	void RunEyeTrackingScript();

	// 눈 추적 파이썬 코드 실행 시킬 지 여부
	bool bIsRunEyeScript{true};

private:
	UPROPERTY()
	FString MapFilePath;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGameStart = false;
};
