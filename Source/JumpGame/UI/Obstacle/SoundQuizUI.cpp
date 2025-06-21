// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundQuizUI.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "JumpGame/Props/ObstacleProp/SoundQuizProp/VoiceRecorderComponent.h"
#include "JumpGame/Utils/FastLogger.h"

// NOTE: 바인딩은 한번만 실행되어야함 NativeConstruct() 에서 실행시 여러번 호출되어 에러 발생
void USoundQuizUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Btn_VoiceSend->OnClicked.AddDynamic(this, &USoundQuizUI::OnClickVoiceSend);	
}

void USoundQuizUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	SetVoiceProgress();
}

void USoundQuizUI::OnClickVoiceSend()
{
	// (5초전에 보내고 싶을때)녹음을 끝내고 AI에게 녹음 파일을 보내자
	// 아무 일도 일어나지 않는다
}

void USoundQuizUI::UpdateFromResponse(uint32 Similarity, FString MessageStr)
{
	FText SimilarityText = FText::AsNumber(Similarity);
	FText MessageText = FText::FromString(MessageStr);
	
	// 받아와서 업데이트 하자
	Text_Similarity->SetText(SimilarityText);
	Text_MessageStr->SetText(MessageText);
}

void USoundQuizUI::SetVoiceRecorderComponent(UVoiceRecorderComponent* VoiceRecorderComp)
{
	if (!VoiceRecorderComp) return;

	VoiceRecorderComponent = VoiceRecorderComp;
	// VoiceRecorderComponent->GetOnStopRecording().AddDynamic(this, &USoundQuizUI::OnClickVoiceSend);
	VoiceRecorderComponent->OnAudioEnvelopeValueNative.AddUObject(this, &USoundQuizUI::OnAudioEnvelopeValue);
}

void USoundQuizUI::OnAudioEnvelopeValue(const class UAudioComponent* AudioComponent,
	const float EnvelopeValue)
{
	// EnvelopeValue값이 0 ~ 0.04정도로 확인됨
	float ScaledValue = EnvelopeValue * 20.0f;
	CurrentEnvelopeValue = FMath::Clamp(ScaledValue, 0.0f, 1.0f);
	// FFastLogger::LogScreen(FColor::Red, TEXT("EnvelopeValue: %f"), EnvelopeValue);
}

void USoundQuizUI::SetVoiceProgress()
{
	VoiceProgressBar->SetPercent(CurrentEnvelopeValue);
}
