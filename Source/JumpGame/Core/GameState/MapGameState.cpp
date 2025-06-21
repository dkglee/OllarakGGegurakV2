// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameState.h"

#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "JumpGame/UI/GameProgressBarUI.h"
#include "JumpGame/UI/InGameSettingUI.h"
#include "JumpGame/UI/LoadingUI.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

AMapGameState::AMapGameState()
{}

void AMapGameState::BeginPlay()
{
	Super::BeginPlay();

	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// 최대 몇명의 플레이어가 플레이를 할지 설정하는 함수 (GI에서 정보를 들고와서 설정해준다)
	int32 MaxPlayer = GI->GetMaxPlayerCount();
	ConnectionVerifyComponent->InitMaxPlayerCount(MaxPlayer);

	ProgressBarUI = CreateWidget<UGameProgressBarUI>(GetWorld(), UGameProgressBarUIClass);
	if (ProgressBarUI)
	{
		ProgressBarUI->AddToViewport();
	}

	InGameSettingUI = CreateWidget<UInGameSettingUI>(GetWorld(), InGameSettingUIClass);
	if (InGameSettingUI)
	{
		InGameSettingUI->AddToViewport();
	}

	APlayerController* PC{UGameplayStatics::GetPlayerController(this, 0)};
	if (PC && PC->IsLocalController())
	{
		if (LoadingUIClass)
		{
			LoadingUI = CreateWidget<ULoadingUI>(PC, LoadingUIClass);
			if (LoadingUI)
			{
				LoadingUI->AddToViewport();
				LoadingUI->InitializeLoadingScreen();
			}
		}
	}

	RisingWaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));
	//RisingWaterProp->StopRising(100);

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = false;
}

void AMapGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 서버에서, 플레이어 위치를 저장하고 알려줄것!
	if (HasAuthority())
	{
		TArray<float> NewPositions;

		for (APlayerState* PS : PlayerArray)
		{
			AController* Controller = Cast<AController>(PS->GetOwner());
			APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;

			if (Pawn)
			{
				NewPositions.Add(Pawn->GetActorLocation().Z);
			}
			else
			{
				NewPositions.Add(0.f);
			}
		}

		MulticastRPC_UpdateAllPlayerZPos(NewPositions);
	}
}

// 서버에서 호출되는 함수
void AMapGameState::OnAllClientAdded()
{
	Super::OnAllClientAdded();

	if (HasAuthority())
	{
		MulticastRPC_UpdateLoadingUI(1.0f);
		// 클라이언트에게 알리자 (2초후)
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMapGameState::MulticastRPC_AllClientAdded, 30.f,
		                                       false);
	}
}

void AMapGameState::OnClientAdded(const FString& NetID)
{
	Super::OnClientAdded(NetID);

	if (HasAuthority())
	{
		UJumpGameInstance* GI{Cast<UJumpGameInstance>(GetWorld()->GetGameInstance())};
		int32 MaxPlayer{GI->GetMaxPlayerCount()};
		int32 CurrentPlayers{PlayerArray.Num()};
		float Progress{0.f};

		if (MaxPlayer > 0)
		{
			Progress = static_cast<float>(CurrentPlayers) / static_cast<float>(MaxPlayer);
		}

		Progress = FMath::Clamp(Progress, 0.0f, 1.0f);

		// 0.5초 있다가 UI에 적용, 너무 바로 되면 이상할까봐
		TWeakObjectPtr<AMapGameState> WeakThis{this};
		FTimerDelegate LoadingDelegate{
			FTimerDelegate::CreateLambda([WeakThis, Progress]() {
				if (WeakThis.IsValid())
				{
					AMapGameState* StrongThis = WeakThis.Get();
					
					StrongThis->MulticastRPC_UpdateLoadingUI(Progress);

					StrongThis->GetWorld()->GetTimerManager().ClearTimer(StrongThis->LoadingTimerHandle);
				}
			})
		};
		
		GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandle, LoadingDelegate, 0.5f, false);
	}
}

void AMapGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(LoadingTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void AMapGameState::MulticastRPC_RemoveProgressBarUI_Implementation()
{
	if (ProgressBarUI)
	{
		ProgressBarUI->RemoveFromParent();
		ProgressBarUI = nullptr;
	}
}

void AMapGameState::MulticastRPC_UpdateAllPlayerZPos_Implementation(
	const TArray<float>& NewPlayerZPos)
{
	AllPlayerZPos = NewPlayerZPos;
}

void AMapGameState::MulticastRPC_AllClientAdded_Implementation()
{
	// 브로드캐스트 하자
	FFastLogger::LogScreen(FColor::Blue, TEXT("모든 클라이언트 들어옴 알림!!"));
	OnAllClientAddedDelegate.Broadcast();

	// 로딩 UI 제거
	MulticastRPC_RemoveLoadingUI();

	//RisingWaterProp->StartRising();
}


void AMapGameState::MulticastRPC_UpdateLoadingUI_Implementation(float Value)
{
	if (LoadingUI)
	{
		LoadingUI->UpdateLoadingProgress(Value);
	}

	APlayerController* PC{UGameplayStatics::GetPlayerController(this, 0)};
	PC->SetInputMode(FInputModeGameOnly());
	PC->bShowMouseCursor = false;
}

void AMapGameState::MulticastRPC_RemoveLoadingUI_Implementation()
{
	if (LoadingUI)
	{
		LoadingUI->RemoveFromParent();
		LoadingUI = nullptr;
	}
}
