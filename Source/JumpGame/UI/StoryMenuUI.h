// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoryMenuUI.generated.h"

class ULobbyCameraComp;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickBackToLobby);

UCLASS()
class JUMPGAME_API UStoryMenuUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UStoryMenuUI(const class FObjectInitializer &ObjectInitializer);
	
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;

	UFUNCTION()
	void OnClickBtn_Back();
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MainStory;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Ep1;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Ep2;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Ep3;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Ep4;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Ep5;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Ep6;

	UFUNCTION()
	void OnClickMainStory();
	// 연잎밥
	UFUNCTION()
	void OnClickEp1();
	// 달빛연못
	UFUNCTION()
	void OnClickEp2();
	// 왕왕벌
	UFUNCTION()
	void OnClickEp3();
	// 삶은당근
	UFUNCTION()
	void OnClickEp4();
	// 개구리동산
	UFUNCTION()
	void OnClickEp5();
	// 연못파리
	UFUNCTION()
	void OnClickEp6();

	// 카메라 전환하기
	UPROPERTY()
	ULobbyCameraComp* CameraComp;

	FOnClickBackToLobby OnClickBackToLobby;

	// 스토리 UI들
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULotusLeafRiceUI> LotusLeafRiceUIClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMoonPondUI> MoonPondUIClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBigBigBee> BigBigBeeUIClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCookedCarrotUI> CookedCarrotUIClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UFrogHill> FrogHillUIClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPondFly> PondFlyUIClass;
};
