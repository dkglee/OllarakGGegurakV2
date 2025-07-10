// Fill out your copyright notice in the Description page of Project Settings.


#include "OutroCinematic.h"
#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "Components/Button.h"
#include "JumpGame/Core/GameMode/ClientRoomGameMode.h"

class AClientRoomGameMode;

void UOutroCinematic::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MediaPlayer && MediaSource)
	{
		MediaPlayer->OpenSource(MediaSource);
		MediaPlayer->Play();
	}
	
	PlayAnimation(SkipAnim);

	// 일정 시간마다 애니메이션 반복 실행
	GetWorld()->GetTimerManager().SetTimer(AnimTimerhandle,	this, &UOutroCinematic::PlaySkipAnim, 3.0f, true);

	Btn_Skip->OnClicked.AddDynamic(this, &UOutroCinematic::OnClickSkip);
}

void UOutroCinematic::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
	{
		if (MediaPlayer->SupportsRate(3.0f, true))
		{
			MediaPlayer->SetRate(3.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("배속 재생이 지원되지 않습니다."));
		}
	}
}

void UOutroCinematic::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimTimerhandle);
	}
	
	Super::NativeDestruct();
}

void UOutroCinematic::PlaySkipAnim()
{
	PlayAnimation(SkipAnim);
}

void UOutroCinematic::OnClickSkip()
{
	AClientRoomGameMode* GM = Cast<AClientRoomGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnVideoEnd();
		GetWorld()->GetTimerManager().ClearTimer(AnimTimerhandle);
	}
}