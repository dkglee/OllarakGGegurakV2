// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	// 플레이어 인덱스
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PlayerID;

	// 플레이어 진짜 이름 (스팀)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerName;

	// 해당 플레이어가 준비 상태인지 여부 (예: 캐릭터 선택완료)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsReady;

	// 현재 생존 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAlive;

	// 승리했는지, 패배했는지
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsWin;

	// 기본 생성자
	FPlayerInfo()
		: PlayerID(-1)
		, PlayerName(TEXT("Unknown"))
		, bIsReady(false)
		, bIsAlive(true)
		, bIsWin(false)
	{}
};


USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MapID;

	// 커스텀 맵 인가요?
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsCustom;
	
	// 맵 썸네일?!
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Thumbnail;

	// 맵마다 맵을 설명하는 UI를 띄우자
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* LoadingWidget;
	
	// 기본 생성자
	FMapInfo()
		: MapID(-1)
		, bIsCustom(false)
		, Thumbnail(nullptr)
		, LoadingWidget(nullptr)
	{}
};


USTRUCT(BlueprintType)
struct FRoomData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString RoomName;

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerCount;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayer;

	UPROPERTY(BlueprintReadWrite)
	FString MapName;

	UPROPERTY(BlueprintReadWrite)
	int32 RoomID;

	UPROPERTY(BlueprintReadWrite)
	bool bIsLocked;

	UPROPERTY(BlueprintReadWrite)
	bool bIsEnded;

	FRoomData()
		: RoomName(TEXT(""))
		, PlayerCount(0)
		, MaxPlayer(4)
		, MapName(TEXT(""))
		, RoomID(-1)
		, bIsLocked(false)
		, bIsEnded(false)
	{}
};

USTRUCT(BlueprintType)
struct FSteamFriendData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly)
	FString SteamId;

	UPROPERTY(BlueprintReadOnly)
	int32 FriendIdx;

	UPROPERTY(BlueprintReadOnly)
	bool bIsOnline;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPlayingThisGame;

	FSteamFriendData()
		: DisplayName(TEXT(""))
		, SteamId(TEXT(""))
		, FriendIdx(-1)
		, bIsOnline(false)
		, bIsPlayingThisGame(false)
	{}
};