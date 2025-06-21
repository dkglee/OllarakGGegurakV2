// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroCinematic.h"
#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "Components/Button.h"
#include "JumpGame/Core/GameMode/ClientRoomGameMode.h"

void UIntroCinematic::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MediaPlayer && MediaSource)
	{
		MediaPlayer->OpenSource(MediaSource);
		MediaPlayer->Play();
	}
	
	PlayAnimation(SkipAnim);

	// 일정 시간마다 애니메이션 반복 실행
	GetWorld()->GetTimerManager().SetTimer(AnimTimerhandle,	this, &UIntroCinematic::PlaySkipAnim, 3.0f, true);

	Btn_Skip->OnClicked.AddDynamic(this, &UIntroCinematic::OnClickSkip);
}

void UIntroCinematic::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UIntroCinematic::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimTimerhandle);
	}
	
	Super::NativeDestruct();
}

void UIntroCinematic::PlaySkipAnim()
{
	PlayAnimation(SkipAnim);
}

void UIntroCinematic::OnClickSkip()
{
	AClientRoomGameMode* GM = Cast<AClientRoomGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnVideoEnd();
		GetWorld()->GetTimerManager().ClearTimer(AnimTimerhandle);
	}
}
