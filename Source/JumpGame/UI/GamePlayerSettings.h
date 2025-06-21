// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "GamePlayerSettings.generated.h"

/**
 * 
 */

UCLASS(Config=Game, BlueprintType)
class JUMPGAME_API UGamePlayerSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	// 사운드
	UPROPERTY(config)
	int32 MasterVolume = 1;
	UPROPERTY(config)
	float BgmVolume = 1.0f;
	UPROPERTY(config)
	float WeatherVolume = 1.0f;
	UPROPERTY(config)
	float UIVolume = 1.0f;
	UPROPERTY(config)
	float CharacterVolume = 1.0f;
	UPROPERTY(config)
	float ObstacleVolume = 1.0f;

	void SetMasterVolume(int32 InMasterVolume) { MasterVolume = InMasterVolume; };
	void SetBgmVolume(float InBgmVolume) { BgmVolume = InBgmVolume; }
	void SetWeatherVolume(float InWeatherVolume) { WeatherVolume = InWeatherVolume; }
	void SetUIVolume(float InUIVolume) { UIVolume = InUIVolume; }
	void SetCharacterVolume(float InCharacterVolume) { CharacterVolume = InCharacterVolume; }
	void SetObstacleVolume(float InObstacleVolume) { ObstacleVolume = InObstacleVolume; }

	int32 GetMasterVolume() const { return MasterVolume; }
	float GetBgmVolume() const { return BgmVolume; }
	float GetWeatherVolume() const { return WeatherVolume; }
	float GetUIVolume() const { return UIVolume; }
	float GetCharacterVolume() const { return CharacterVolume; }
	float GetObstacleVolume() const { return ObstacleVolume; }
	
	// 화면 밝기
	UPROPERTY(config)
	float Brightness = 1.0f;
	UPROPERTY(config)
	float Contrast = 1.0f;

	void SetBrightness(float InBrightness) { Brightness = InBrightness; }
	void SetContrast(float InContrast) { Contrast = InContrast; }

	float GetBrightness() const { return Brightness; }
	float GetContrast() const { return Contrast; }

	// 안티엘리어싱
	UPROPERTY(config)
	int32 AntiValue = 0;

	void SetAntiValue(int32 InAntiValue) { AntiValue = InAntiValue; }
	int32 GetAntiValue() const { return AntiValue; }
	
	// 색각이상
	UPROPERTY(config)
	int32 ColorMode = 0;

	void SetColorMode(int32 InColorMode) { ColorMode = InColorMode; }
	int32 GetColorMode() const { return ColorMode; }
	
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
};
