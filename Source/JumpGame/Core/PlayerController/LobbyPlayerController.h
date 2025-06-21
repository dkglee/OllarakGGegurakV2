// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkPlayerController.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "LobbyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextChatTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendListReceived, const TArray<FSteamFriendData>&, FriendList);

/**
 * 
 */
UCLASS()
class JUMPGAME_API ALobbyPlayerController : public ANetworkPlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController();
	
public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* NextChatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputMappingContext* LobbyMappingContext;

	// NextChatAction 시 호출될 함수
	void NextChat();

	UPROPERTY(BlueprintAssignable)
	FOnNextChatTriggered OnNextChatTriggered;
public:
	
	/*
	 * Return : 0 : 실패, 1 : 성공
	 * OutPitch : Pitch 비율 값
	 * OutRoll : Roll 비율 값
	 */
	UFUNCTION()
	int32 GetPitchYawRatio(float& OutPitch, float& OutRoll);

	UFUNCTION()
	float GetPitchRatio(FVector InLocallyPosition, FVector InLocallyMousePosition);
	UFUNCTION()
	float GetYawRatio(FVector InLocallyPosition, FVector InLocallyMousePosition);

private:
	UFUNCTION()
	void CalculateMinMax(const FVector& InLocallyPosition);

	// Min : X 좌표, Max : Y 좌표
	UPROPERTY()
	FVector2D MinusPitchMinMax = FVector2D(0.f, 0.f);
	UPROPERTY()
	FVector2D MinusYawMinMax = FVector2D(0.f, 0.f);
	UPROPERTY()
	FVector2D PlusPitchMinMax = FVector2D(0.f, 0.f);
	UPROPERTY()
	FVector2D PlusYawMinMax = FVector2D(0.f, 0.f);

public:
	// 팝업으로 띄울 UI
	UPROPERTY(editanywhere)
	TSubclassOf<class UBottomNaviBarUI> BottomNaviBarUIClass;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	UBottomNaviBarUI* BottomNaviBarUI;

public:
	/** 서버 → 클라 : 친구 목록 보내 달라 */
	UFUNCTION(Client, Reliable)
	void ClientRPC_RequestFriendList();

	/** 클라 → 서버 : 친구 목록 전달 */
	UFUNCTION(Server, Reliable)
	void ServerRPC_SendFriendList(const TArray<FSteamFriendData>& FriendList);

	/** 서버 → 모든(또는 선택) 클라 : 전달받은 목록 브로드캐스트 */
	UFUNCTION(Client, Reliable)
	void ClientRPC_ReceiveFriendList(const FUniqueNetIdRepl& FromPlayer,
									 const TArray<FSteamFriendData>& FriendList);
	void Recursive_ReceiveFriendList(const FUniqueNetIdRepl& FromPlayer,
									 const TArray<FSteamFriendData>& FriendList);

	/** UI 바인딩용 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnFriendListReceived OnFriendListUpdated;

private:
	/** 내가 최종적으로 받은(또는 보낸) 친구 목록 */
	UPROPERTY()
	TArray<FSteamFriendData> MyFriendList;

public:
	/** UI 에서 호출할 로컬 전용 함수 */
	UFUNCTION(BlueprintCallable, Category="Friends")
	void RequestFriendList();

private:
	/** 실제 Steam 친구 읽기 + 서버 전송 로직 */
	void FetchAndSendFriendList();
};
