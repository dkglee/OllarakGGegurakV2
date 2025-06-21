// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitRoomUI.generated.h"

/*
여기는 대기방 (유저들이 입장해서 대기하는 방)
-> 서버가 게임 시작 버튼 클릭 시 레벨전환
 */
UCLASS()
class JUMPGAME_API UWaitRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnMapSelected(class UMapSlotUI* MapSlotUI);
	UFUNCTION()
	void UpdateCurrentMapThumbnail(UTexture2D* Texture);
	UFUNCTION()
	void UpdateCurrentMapName(const FString& MapName);

	UPROPERTY()
	class UJumpGameInstance* GI;
	
	// 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameStart;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SelectMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromLobby;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MapName;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Selected;
	

	// 클릭시 실행될 함수
	UFUNCTION()
	void OnClickGameStart();
	UFUNCTION()
	void OnClickSelectMap();
	UFUNCTION()
	void OnClickBackFromLobby();

	// 팝업으로 띄울 UI
	UPROPERTY(editanywhere)
	TSubclassOf<class USelectRoomUI> SelectRoomFactory;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	USelectRoomUI* SelectRoomUI;

	// 유저가 들어오면 업데이트 될 이미지 배열
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user3;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user4;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user5;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_user6;

	// 이름 배열
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player1;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player2;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player3;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player4;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player5;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Player6;

	// 이미지 업데이트 함수
	void UpdateWaitPlayer();

	// 맵 선택 창 애니메이션 재생
	UFUNCTION(BlueprintNativeEvent)
	void ShowMapSelect();

	// 카메라 전환 관련
	UPROPERTY(editanywhere)
	class APlayerController* PC;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	class ALobbySubCamera* SubCamera;

	UPROPERTY()
	FSlateColor DefaultTintColor;

	UPROPERTY()
	TObjectPtr<class UTexture2D> CurrentMapThumbnail;
};
