// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "GameProgressBarUI.generated.h"

/**
 * 게임 진행도를 나타내는 프로그레스바
 * 추가사항: 각 플레이어 이동 정보, 물의 위치값
 */
UCLASS()
class JUMPGAME_API UGameProgressBarUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry & MyGeometry, float DeltaSeconds) override;

	// UI
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_Player;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* GameProgressBar;
	
	UPROPERTY()
	float GameProgress = 0;
	UPROPERTY()
	float StartPropZ = 0;
	UPROPERTY()
	float FinPropZ = 0;
	UPROPERTY()
	float WaterPropZ = 0;

	// 업데이트 값들
	UPROPERTY()
	class UJumpGameInstance* GI;
	UPROPERTY()
	class AFrog* Character;
	UPROPERTY()
	TMap<FString, FPlayerInfo> PlayerInfo;
	// 배열에 포함된 플레이어 수
	UPROPERTY()
	int32 MaxPlayerID = -1;
	// 내 플레이어의 인덱스(ID)
	UPROPERTY()
	uint32 MyPlayerID = -1;
	UPROPERTY()
	FString PlayerKey;
	
	// 나를 포함한 모든 플레이어의 위치
	UPROPERTY()
	TArray<float> PlayerPos = { 0 };
	// 1등 플레이어의 위치
	UPROPERTY()
	float WinnerPos = 0;
	UPROPERTY()
	float TotalGamePos = 0;

	UFUNCTION()
	void UpdatePlayerPos();
	UFUNCTION()
	void UpdateProgressBar();

	UFUNCTION()
	void CreatePlayerMarkers();
	UFUNCTION()
	void UpdatePlayerMarkers();

	// 모든 정보가 업데이트 되었을때 호출될 함수
	UFUNCTION()
	void InitUISetting();

	// 플레이어  Marker 위젯 추가
	UPROPERTY(editanywhere)
	TSubclassOf<class UPlayerMarkerWidget> PlayerMarkerFactory;
	UPROPERTY(editanywhere)
	UPlayerMarkerWidget* PlayerMarker;
	// 플레이어 목록 배열
	UPROPERTY(editanywhere)
	TArray<UPlayerMarkerWidget*> PlayerMarkers;
	
	// 업데이트 됐나요?
	UPROPERTY()
	bool bIsInit = false;

	// 눈에 보이나요?
	UPROPERTY()
	bool bIsShow = true;

	// 높이에 따른 업데이트
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSunBrightness(float Value);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFogDensity(float Value);
};
