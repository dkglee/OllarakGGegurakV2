// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoundQuizUI.generated.h"

class UVoiceRecorderComponent;
/**
 * 
 */
UCLASS()
class JUMPGAME_API USoundQuizUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	//타이머 UI
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTimeRemainUI* WBP_TimeRemain;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_VoiceSend;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_VoiceSend;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Similarity;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_MessageStr;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* VoiceProgressBar;
	UPROPERTY()
	float CurrentEnvelopeValue = 0.f;

	UFUNCTION()
	void OnClickVoiceSend();
	UFUNCTION()
	void UpdateFromResponse(uint32 Similarity, FString MessageStr);
	
	// 녹음 컴포넌트 동작
	UPROPERTY()
	TWeakObjectPtr<UVoiceRecorderComponent> VoiceRecorderComponent = nullptr;

	UFUNCTION()
	void SetVoiceRecorderComponent(UVoiceRecorderComponent* VoiceRecorderComp);
	// 녹음 중인지 확인
	UFUNCTION()
	void OnAudioEnvelopeValue(const class UAudioComponent* AudioComponent, const float EnvelopeValue);

	UFUNCTION()
	void SetVoiceProgress();
};
