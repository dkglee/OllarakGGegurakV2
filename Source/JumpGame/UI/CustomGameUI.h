// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomGameUI.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickBackToLobby);

UCLASS()
class JUMPGAME_API UCustomGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;
	UFUNCTION()
	void OnClickBtn_Back();

	// 카메라 전환하기
	UPROPERTY()
	class ULobbyCameraComp* CameraComp;
	FOnClickBackToLobby OnClickBackToLobby;

public:
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
	class UTextBlock* Text_MapName;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Selected;
	
	// 클릭시 실행될 함수
	UFUNCTION()
	void OnClickGameStart();
	UFUNCTION()
	void OnClickSelectMap();

	// 팝업으로 띄울 UI
	UPROPERTY(editanywhere)
	TSubclassOf<class USelectRoomUI> SelectRoomFactory;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	USelectRoomUI* SelectRoomUI;

	// 맵 선택 창 애니메이션 재생
	UFUNCTION(BlueprintNativeEvent)
	void ShowMapSelect();

	// 카메라 전환 관련
	UPROPERTY()
	FSlateColor DefaultTintColor;

	UPROPERTY()
	TObjectPtr<class UTexture2D> CurrentMapThumbnail;
};
