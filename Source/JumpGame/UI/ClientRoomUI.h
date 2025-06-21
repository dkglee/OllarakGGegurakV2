// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "UICam/LobbyCameraComp.h"
#include "ClientRoomUI.generated.h"

/*
여기서는 로비를 생성하고 찾는다
 -> 즉, 게임 시작시 메인화면

WidgetSwitcher 구조
0) 메인화면: 게임 시작시 보이는 화면
1) FindRoom: 메인화면에서 방 목록보기를 누르면 이동하는 화면
-> FindRoom에서 팝업으로 방을 생성한다
 */

class UButton;
class UImage;

UCLASS()
class JUMPGAME_API UClientRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY()
	class UJumpGameInstance* GI;
	
public:
	// 위젯 스위처
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	// WidgetSwitcher (0)	
	// 메인 관련
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasMain;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoFind;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoCreateMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoStoryMenu;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoSettings;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoCredit;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoGameEnd;
	
	// 눌렀을때 호출되는 함수
	UFUNCTION()
	void OnClickGoFindRoom();
	UFUNCTION()
	void OnClickGoCreateMap();
	UFUNCTION()
	void OnClickGoStoryMenu();
	UFUNCTION()
	void OnClickGoSettings();
	UFUNCTION()
	void OnClickGoCredit();
	UFUNCTION()
	void OnClickGoGameEnd();

	// WidgetSwitcher (1)
	// 방 만들기 관련
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasCreateRoom;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoCreate;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Edit_RoomName;
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_PlayerCnt;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerCnt;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Create;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromCreate;

	// 비밀방 설정하기
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* CheckBox_Lock;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Edit_Password;
	
	// 버튼 눌렀을 때 호출될 함수
	UFUNCTION()
	void OnClickGoCreateRoom();
	UFUNCTION()
	void CreateSession();
	// 비밀번호 설정 함수
	UFUNCTION()
	void OnClickCheckBox(bool bIsChecked);
	
	// 슬라이더
	UFUNCTION()
	void OnValueChanged(float Value);
	
	// 세션 검색 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Find;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromFind;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_RoomList;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_BtnFind;

	// 눌렀을 때 호출되는 함수
	UFUNCTION()
	void OnClickBackFromFind();
	
	// 세션 목록
	UPROPERTY(editanywhere)
	TSubclassOf<class USessionListItemWidget> SessionItemFactory;
	UPROPERTY(editanywhere)
	TSubclassOf<class USessionListItemDouble> SessionItemDoubleFactory;
	
	// 오브젝트 풀
	UPROPERTY(editanywhere)
	TArray<USessionListItemWidget*> RoomListPool;
	UPROPERTY(editanywhere)
	TArray<USessionListItemDouble*> RoomDoubleWidgets;
	// 찾아진 방 정보 저장
	UPROPERTY(editanywhere)
	TArray<FRoomData> AllFoundRooms;
	// 최대 방 개수
	UPROPERTY(editanywhere)
	int32 MaxRoomCount = 50; 

	UFUNCTION()
	void UpdateRoomList(const TArray<FRoomData>& RoomDataArray);
	UFUNCTION()
	void InitRoomListPool();
	
	// 버튼 클릭 함수
	UFUNCTION()
	void OnClickFind();
	UFUNCTION()
	void OnClickBack();

	// 델리게이트 함수
	void OnFindComplete(const FRoomData& Data);

	// 카메라 전환하기
	UPROPERTY()
	ULobbyCameraComp* CameraComp;

public:
	// 스토리 UI 띄우기
	UPROPERTY(editanywhere, BlueprintReadWrite)
	TSubclassOf<class UStoryMenuUI> StoryMenuUIClass;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	UStoryMenuUI* StoryMenuUI;

	// 보이니?
	UPROPERTY()
	ESlateVisibility bIsVisible;
	UFUNCTION()
	void SetVisibleMain();

public:
	// 세팅 UI 띄우기
	UPROPERTY(editanywhere, BlueprintReadWrite)
	TSubclassOf<class UGameSettingUI> GameSettingUIClass;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	UGameSettingUI* GameSettingUI;
	
	// 크레딧 UI 띄우기
	UPROPERTY(editanywhere, BlueprintReadWrite)
	TSubclassOf<class UCreditUI> CreditUIClass;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	UCreditUI* CreditUI;
};

