// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "SessionListItemWidget.generated.h"

/*
 세션 생성 시 보여지는 제목 리스트
 현재 인원 수, 게임 진행중 여부?, 비밀방인지 아닌지
 */
UCLASS()
class JUMPGAME_API USessionListItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Join;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RoomInfo;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MaxPlayer;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MapName;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Secret;

	UPROPERTY()
	int32 RoomIdx;
	
	UFUNCTION()
	void OnClickJoin();
	UFUNCTION()
	void SetRoomInfo(const FRoomData& Data);
};
