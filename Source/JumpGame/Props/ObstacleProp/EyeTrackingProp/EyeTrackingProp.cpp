// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeTrackingProp.h"
#include "JumpGame/UI/Obstacle/SettingEyeTrackingUI.h"

#include "Components/BoxComponent.h"
#include "JumpGame/AIServices/Shared/IOManagerComponent.h"
#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AEyeTrackingProp::AEyeTrackingProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<USettingEyeTrackingUI> SettingEyeTrackingUIWidget
		(TEXT("/Game/UI/Obstacle/WBP_SettingEyeTracking.WBP_SettingEyeTracking_C"));
	if (SettingEyeTrackingUIWidget.Succeeded())
	{
		SettingEyeTrackingUIClass = SettingEyeTrackingUIWidget.Class;
	}
	
	CollisionComp->SetBoxExtent(FVector(40.f, 40.f, 5.f));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

// Called when the game starts or when spawned
void AEyeTrackingProp::BeginPlay()
{
	Super::BeginPlay();

	SettingEyeTrackingUI = CreateWidget<USettingEyeTrackingUI>(GetWorld(), SettingEyeTrackingUIClass);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEyeTrackingProp::SendEyeTrackingSettingStart,3.f, false);

	//SendEyeTrackingSettingStart();
}

void AEyeTrackingProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		SendEyeTrackingStart();
	}
}

void AEyeTrackingProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		SendEyeTrackingEnd();
	}
}

void AEyeTrackingProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
}

// Called every frame
void AEyeTrackingProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
	// {
	// 	RecvReadyEyeTracking();
	// }
	//
	// if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
	// {
	// 	SendEyeTrackingSettingStart();
	// }
	
	if (!bIsStartHunt)
	{
		return;
	}
	
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		SendEyeTrackingStart();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
		SendEyeTrackingEnd();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
	{
		RecvEyeTrackingInfo();
	}

}

void AEyeTrackingProp::RecvReadyEyeTracking()
{
	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	FMessageUnion RecvMessage;
	if (GS->IOManagerComponent->PopMessage(EMessageType::EyeTrackingRequestMessage, RecvMessage))
	{
		QuizID = RecvMessage.EyeTrackingRequestMessage.QuizID;
		Width = RecvMessage.EyeTrackingRequestMessage.Width;
		Height = RecvMessage.EyeTrackingRequestMessage.Height;
		Start = RecvMessage.EyeTrackingRequestMessage.Start;
		End = RecvMessage.EyeTrackingRequestMessage.End;
	}

	if (End == 0)
	{
		SettingEyeTrackingUI->RemoveFromParent();

		bIsSettingFinish = true;
	}
	
	if (GEngine && GEngine->GameViewport && GEngine->GameUserSettings)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		
		SettingLocation[0] = {50.f, 50.f};
		SettingLocation[1] = {ViewportSize.X - 50.f, 50.f};
		SettingLocation[2] = {ViewportSize.X - 50.f, ViewportSize.Y - 50.f};
		SettingLocation[3] = {50.f, ViewportSize.Y - 50.f};
	}
	
	if (SettingEyeTrackingUI)
	{
		if (!SettingEyeTrackingUI->IsInViewport())
		{
			SettingEyeTrackingUI->AddToViewport();
		}

		SettingEyeTrackingUI->SetPositionInViewport(SettingLocation[SettingCount]);
		++SettingCount;
	}
}

void AEyeTrackingProp::SendEyeTrackingStart()
{
	FLog::Log("SendEyeTrackingStart");

	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	FMessageUnion SendMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.Type = EMessageType::EyeTrackingNotifyMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.PayloadSize = sizeof(FEyeTrackingNotifyMessage);
	SendMessage.EyeTrackingNotifyMessage.Header.SessionID[0] = 1;
	SendMessage.EyeTrackingNotifyMessage.Header.PlayerID = 1;

	SendMessage.EyeTrackingNotifyMessage.QuizID = 100;
	SendMessage.EyeTrackingNotifyMessage.SettingStart = 0;
	SendMessage.EyeTrackingNotifyMessage.Start = 1;
	SendMessage.EyeTrackingNotifyMessage.End = 0;

	GS->IOManagerComponent->SendGameMessage(SendMessage);
}

void AEyeTrackingProp::SendEyeTrackingEnd()
{
	FLog::Log("SendEyeTrackingEnd");

	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	FMessageUnion SendMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.Type = EMessageType::EyeTrackingNotifyMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.PayloadSize = sizeof(FEyeTrackingNotifyMessage);
	SendMessage.EyeTrackingNotifyMessage.Header.SessionID[0] = 1;
	SendMessage.EyeTrackingNotifyMessage.Header.PlayerID = 1;

	SendMessage.EyeTrackingNotifyMessage.QuizID = 100;
	SendMessage.EyeTrackingNotifyMessage.SettingStart = 0;
	SendMessage.EyeTrackingNotifyMessage.Start = 0;
	SendMessage.EyeTrackingNotifyMessage.End = 1;

	GS->IOManagerComponent->SendGameMessage(SendMessage);
}

void AEyeTrackingProp::SendEyeTrackingSettingStart()
{
	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	FMessageUnion SendMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.Type = EMessageType::EyeTrackingNotifyMessage;
	SendMessage.EyeTrackingNotifyMessage.Header.PayloadSize = sizeof(FEyeTrackingNotifyMessage);
	SendMessage.EyeTrackingNotifyMessage.Header.SessionID[0] = 1;
	SendMessage.EyeTrackingNotifyMessage.Header.PlayerID = 1;

	SendMessage.EyeTrackingNotifyMessage.QuizID = 100;
	SendMessage.EyeTrackingNotifyMessage.SettingStart = 1;
	SendMessage.EyeTrackingNotifyMessage.Start = 0;
	SendMessage.EyeTrackingNotifyMessage.End = 0;

	GS->IOManagerComponent->SendGameMessage(SendMessage);
}

void AEyeTrackingProp::RecvEyeTrackingInfo()
{
	//FLog::Log("RecvEyeTrackingInfo");
	const ANetworkGameState* GS{Cast<ANetworkGameState>(GetWorld()->GetGameState())};

	if (!GS)
	{
		FLog::Log("No GameState");
		return;
	}

	FMessageUnion RecvMessage;
	if (GS->IOManagerComponent->PopMessage(EMessageType::EyeTrackingResponseMessage, RecvMessage))
	{
		QuizID = RecvMessage.EyeTrackingResponseMessage.QuizID;
		X = RecvMessage.EyeTrackingResponseMessage.X;
		Y = RecvMessage.EyeTrackingResponseMessage.Y;
		bBlink = RecvMessage.EyeTrackingResponseMessage.bBlink;
		State = RecvMessage.EyeTrackingResponseMessage.State;

		// UE_LOG(LogTemp, Warning, TEXT("State: %d"), State);
		// UE_LOG(LogTemp, Warning, TEXT("Blink: %d"), bBlink);
		//UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f"), X, Y);
	}
	//UE_LOG(LogTemp, Warning, TEXT("QuizID: %d"), QuizID);
}