// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomGameState.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "JumpGame/UI/BottomNaviBarUI.h"
#include "JumpGame/UI/FriendsList.h"
#include "JumpGame/UI/Cinematic/IntroCinematic.h"
#include "JumpGame/UI/Lobby/Follower.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class ALobbyPlayerController;

AClientRoomGameState::AClientRoomGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	PrevLoc = FVector2D::ZeroVector;
}

void AClientRoomGameState::BeginPlay()
{
	Super::BeginPlay();

	FollowerUI = CreateWidget<UFollower>(GetWorld(), FollowerUIClass);
	if (FollowerUI)
	{
		FollowerUI->AddToViewport();
		APlayerController* PC{UGameplayStatics::GetPlayerController(this, 0)};
		PrevLoc = UWidgetLayoutLibrary::GetMousePositionOnViewport(PC);
	}
}

void AClientRoomGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PC{UGameplayStatics::GetPlayerController(this, 0)};
	FVector2D MousePos{};
	PC->GetMousePosition(MousePos.X, MousePos.Y);
	MousePos -= {-10.f, 25.f};

	if (FollowerUI)
	{
		PrevLoc = FMath::Lerp(PrevLoc, MousePos, 0.1);
		FollowerUI->SetPositionInViewport(PrevLoc);

		// float CurrentAngle{FollowerUI->GetRenderTransformAngle()};
		// float TargetAngle{
		// 	static_cast<float>(UKismetMathLibrary::DegAtan2((PrevLoc - MousePos).Y, (PrevLoc - MousePos).X))
		// };
		// TargetAngle = UKismetMathLibrary::FClamp(TargetAngle, -80.f, 80.f);
		// CurrentAngle = FMath::Lerp(CurrentAngle, TargetAngle, 0.3f);
		// // FollowerUI->SetRenderTransformAngle(
		// // 	UKismetMathLibrary::FInterpTo(CurrentAngle, TargetAngle, DeltaSeconds / 10.f, 10.f));
		// FollowerUI->SetRenderTransformAngle(CurrentAngle);
		//
		// FLog::Log("C, T", CurrentAngle, TargetAngle);
		
		FVector2D MouseVelocity{MousePos - PrevMousePos};
		// 마우스 속도에 따른 각도 크기
		float TargetAngle{static_cast<float>(FMath::Clamp(MouseVelocity.Size() * 3.f, 0.f, 80.f))};
		// 마우스 이동 방향에 따른 각도 부호 설정
		if (MouseVelocity.X < 0)
		{
			TargetAngle *= -1.f;
		}

		AngleVelocity = FMath::Lerp(AngleVelocity, TargetAngle, 0.1f);
		FollowerUI->SetRenderTransformAngle(AngleVelocity);

		PrevMousePos = MousePos;
	}
}

void AClientRoomGameState::OnConnectionSucceeded()
{
	Super::OnConnectionSucceeded();

	// // PlayerController에 델리게이트 바인딩
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		UFriendsList* FriendList = PC->BottomNaviBarUI->FriendsList;
		PC->OnFriendListUpdated.AddDynamic(FriendList, &UFriendsList::OnFriendListReceived);
	
		// 자동 요청
		PC->RequestFriendList();
		FriendList->Btn_Refresh->SetIsEnabled(false);
	}
}
