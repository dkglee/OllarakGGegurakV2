// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "SelectRoomUI.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnMapSelected, class UMapSlotUI*, MapSlotUI);

UCLASS()
class JUMPGAME_API USelectRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void InitSelectRoomUI();

	// 맵 선택 창 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoBackWait;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SelectComplete;

	// 맵 종류 선택 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AllMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_OriginMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CustomMap;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_PickFile;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_AllMap;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_OriginMap;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_CustomMap;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MapName;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Selected;

	// 버튼 클릭시 실행 함수
	UFUNCTION()
	void OnClickGoBackWait();
	UFUNCTION()
	void OnClickSelectComplete();
	UFUNCTION()
	void OnClickAllMap();
	UFUNCTION()
	void OnClickOriginMap();
	UFUNCTION()
	void OnClickCustomMap();
	UFUNCTION()
	void OnPickCustomMap();
	UFUNCTION()
	void OnMapClicked(class UMapSlotUI* MapSlotUI);
	void OnPickFileComplete(const FString& FilePath, bool bSuccess);

	UFUNCTION()
	TArray<FString> GetMapList(const FString& MapType, const FString& MapDir);
	
	// 맵 리스트 관련
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_SR;

	UPROPERTY()
	FOnMapSelected OnMapSelectedDelegate;

	GETTER(class UMapSlotUI*, CurrentSelectedMapSlotUI)

private:
	void InitAllMap();
	void InitOriginMap();
	void InitCustomMap();
	
	UFUNCTION()
	void FillScrollBox(class UScrollBox* ScrollBox, const TArray<UMapSlotUI*>& MapList);
	
	UPROPERTY()
	TArray<FString> OriginMapList;
	UPROPERTY()
	TArray<FString> CustomMapList;
	UPROPERTY()
	TArray<FString> SavedMapList;
	UPROPERTY()
	TArray<FString> CombinedMapList;

	UPROPERTY(meta = (AllowPrivateAccess = "true"), BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UMapSlotUI> MapSlotClass;

	UPROPERTY()
	TArray<class UMapSlotUI*> OriginMapListUI;
	UPROPERTY()
	TArray<class UMapSlotUI*> CustomMapListUI;
	UPROPERTY()
	TArray<class UMapSlotUI*> CombinedMapListUI;
	
	UPROPERTY()
	class UMapSlotUI* PreviewSelectedMapSlotUI;
	UPROPERTY()
	class UMapSlotUI* CurrentSelectedMapSlotUI;
	UPROPERTY()
	TArray<class UHorizontalBox*> MapSlotBoxes;
	int32 HorizontalBoxCount = 10;
	bool bIsInitialized = false;

	bool bPreviewByPickedFile = false;
	bool bCurrentByPickedFile = false;

	UPROPERTY()
	FSlateColor DefaultTintColor;
};
