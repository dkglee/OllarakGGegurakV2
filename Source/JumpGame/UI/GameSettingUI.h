// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerSettings.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Characters/Frog.h"
#include "GameSettingUI.generated.h"

/**/

UENUM(BlueprintType)
enum class EColorBlindMode : uint8
{
	None       UMETA(DisplayName = "보정 없음"),
	Deuteranope UMETA(DisplayName = "녹색약"),
	Protanope  UMETA(DisplayName = "적색약"),
	Tritanope  UMETA(DisplayName = "청색약"),
};

UCLASS()
class JUMPGAME_API UGameSettingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	// 세팅 저장
	UPROPERTY()
	UGamePlayerSettings* Settings;
	UFUNCTION()
	void InitUISettings();
	
	// 나왔다 들어갔다 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SettingAnim;
	UFUNCTION()
	void PlaySettingAnim(bool bIsForward);

	// 전환
	UPROPERTY()
	UUMGSequencePlayer* SCPlayer;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SettingSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameSet;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_OtherSet;

	UFUNCTION()
	void OnClickGameSet();
	UFUNCTION()
	void OnClickOtherSet();

	// 사운드 클라스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundMix* MasterSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundClass* SC_Master;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundClass* SoundClass_Bgm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundClass* SoundClass_InGameEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundClass* SoundClass_Nature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundClass* SoundClass_Obstacle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundClass* SoundClass_UI;

	// WidgetSwitcher(0)
	// 버튼별 기능_사운드
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SoundOn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SoundOff;

	UFUNCTION()
	void OnClickSoundOn();
	UFUNCTION()
	void OnClickSoundOff();

	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Bgm;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Weather;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_UI;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Character;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Obstacle;

	UFUNCTION()
	void OnBgmValueChanged(float Value);
	UFUNCTION()
	void OnWeatherValueChanged(float Value);
	UFUNCTION()
	void OnUIValueChanged(float Value);
	UFUNCTION()
	void OnCharacterValueChanged(float Value);
	UFUNCTION()
	void OnObstacleValueChanged(float Value);

	// 환경 효과 (인게임 내의 액터가 가지고 있다)
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_WeatherOn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_WeatherOff;
	
	UFUNCTION()
	void OnClickWeatherOn();
	UFUNCTION()
	void OnClickWeatherOff();

	// 밝기 (캐릭터가 가지고 있다)
	UPROPERTY()
	AFrog* Character;
	UPROPERTY(meta = (BindWidget))
	class USlider* Sd_Light;
	
	UFUNCTION()
	void OnLightValueChanged(float Value);
	
	// 안티엘리어싱
	UPROPERTY()
	TMap<FString, int32> AntiAliasingMap;
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox_Anti;
	UFUNCTION()
	void SetAntiAliasingQuality(const FString& SelectedOption);
	UFUNCTION()
	void OnAntiModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	// WidgetSwitcher(1)
	// 색약설정
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox_Color;
	UPROPERTY()
	TMap<FString, EColorBlindMode> ColorBlindMap;
	UFUNCTION()
	void OnColorModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// 뒤로가기
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_QuitGame;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoBack;

	UFUNCTION()
	void OnClickQuitGame();
	UFUNCTION()
	void OnClickGoBack();

	// 게임종료 UI 띄우기
	UPROPERTY(editanywhere, BlueprintReadWrite)
	TSubclassOf<class UGameQuitUI> GameQuitUIClass;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	UGameQuitUI* GameQuitUI;
};
