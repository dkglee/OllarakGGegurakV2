// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGameState.h"

#include "Components/Image.h"
#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/PlayerController/InGamePlayerController.h"
#include "JumpGame/Maps/Node/StageNodeActor.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "JumpGame/UI/GameProgressBarUI.h"
#include "JumpGame/UI/InGameSettingUI.h"
#include "JumpGame/UI/LevelTransfer.h"
#include "JumpGame/UI/LoadingUI.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"

AMapGameState::AMapGameState()
{
	SaveMapComponent = CreateDefaultSubobject<USaveMapComponent>(TEXT("SaveMapComponent"));

	ConstructorHelpers::FClassFinder<ULevelTransfer> WidgetTransferUIWidget
	(TEXT("/Game/UI/LobbyUI/WBP_LevelTransfer.WBP_LevelTransfer_C"));
	if (WidgetTransferUIWidget.Succeeded())
	{
		WidgetTransferUIClass = WidgetTransferUIWidget.Class;
	}
}

void AMapGameState::BeginPlay()
{
	Super::BeginPlay();
	
	// FString MapName = GetWorld()->GetMapName();
	// MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	// FFastLogger::LogConsole(TEXT("MapName : %s"), *MapName);
	// SaveMapComponent->SaveMap(MapName, TEXT(""));

	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// 최대 몇명의 플레이어가 플레이를 할지 설정하는 함수 (GI에서 정보를 들고와서 설정해준다)
	int32 MaxPlayer = GI->GetMaxPlayerCount();
	ConnectionVerifyComponent->InitMaxPlayerCount(MaxPlayer);

	// ProgressBarUI = CreateWidget<UGameProgressBarUI>(GetWorld(), UGameProgressBarUIClass);
	// if (ProgressBarUI)
	// {
	// 	ProgressBarUI->AddToViewport();
	// }

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
				LoadingUI->AddToViewport(2);
				LoadingUI->InitializeLoadingScreen();
			}
		}
	}

	RisingWaterProp = Cast<ARisingWaterProp>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));
	//RisingWaterProp->StopRising(100);

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = false;

	// 별 관련
	StarCount = 0;

	// 시간 기록
	StartTime = GetWorld()->GetTimeSeconds();

	if (WidgetTransferUIClass)
	{
		WidgetTransferUI = CreateWidget<ULevelTransfer>(GetWorld(), WidgetTransferUIClass);
		WidgetTransferUI->AddToViewport(1);
		WidgetTransferUI->FillViewport();
	}

	RadiusValue = 3.f;

	if (UStageSystemSubsystem* SS = GetGameInstance()->GetSubsystem<UStageSystemSubsystem>())
	{
		if (SS->GetFieldState(SS->GetChosenField()) == EFieldProgressState::Cleared) return; // 이미 클리어한 맵이면 업데이트 하지말자)
		SS->SetFieldState(SS->GetChosenField(), EFieldProgressState::InProgress);
	}
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

	// if (HasAuthority())
	// {
	// 	MulticastRPC_UpdateLoadingUI(1.0f);
	// 	// 클라이언트에게 알리자 (2초후)
	// 	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMapGameState::MulticastRPC_AllClientAdded, 2.f,
	// 	                                       false);
	// }
}

void AMapGameState::OnClientAdded(const FString& NetID)
{
	Super::OnClientAdded(NetID);

	if (HasAuthority())
	{
		// UJumpGameInstance* GI{Cast<UJumpGameInstance>(GetWorld()->GetGameInstance())};
		// int32 MaxPlayer{GI->GetMaxPlayerCount()};
		// int32 CurrentPlayers{PlayerArray.Num()};
		// float Progress{0.f};
		//
		// if (MaxPlayer > 0)
		// {
		// 	Progress = static_cast<float>(CurrentPlayers) / static_cast<float>(MaxPlayer);
		// }
		//
		// Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
		//
		// // 0.5초 있다가 UI에 적용, 너무 바로 되면 이상할까봐
		// TWeakObjectPtr<AMapGameState> WeakThis{this};
		// FTimerDelegate LoadingDelegate{
		// 	FTimerDelegate::CreateLambda([WeakThis, Progress]()
		// 	{
		// 		if (WeakThis.IsValid())
		// 		{
		// 			AMapGameState* StrongThis = WeakThis.Get();
		//
		// 			StrongThis->MulticastRPC_UpdateLoadingUI(Progress);
		//
		// 			StrongThis->GetWorld()->GetTimerManager().ClearTimer(StrongThis->LoadingTimerHandle);
		// 		}
		// 	})
		// };
		//
		// GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandle, LoadingDelegate, 0.5f, false);
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

void AMapGameState::RemoveLoadingUI()
{
	if (LoadingUI)
	{
		LoadingUI->RemoveFromParent();
		LoadingUI = nullptr;
	}
	
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &AMapGameState::TransitionAnimation,
									   0.016f, true);
}

void AMapGameState::TransitionAnimation()
{
	RadiusValue -= 0.02f * 2.f;

	UMaterialInstanceDynamic* DynamicMaterial{WidgetTransferUI->Image_Circle->GetDynamicMaterial()};
	DynamicMaterial->SetScalarParameterValue("Radius", RadiusValue);
	
	if (RadiusValue <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TransitionTimer);
		WidgetTransferUI->RemoveFromParent();

		AInGamePlayerController* PC{(Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))};
		if (PC)
		{
			PC->ShowInGameUI();
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMapGameState::AddStar()
{
	StarCount++;
	
	AInGamePlayerController* PC{(Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))};
	if (PC)
	{
		PC->UpdateStarCount(StarCount);
	}

	if (StarCount >= MaxStarCount)
	{
		EndStage(true);
	}
}

void AMapGameState::EndStage(bool bIsClear)
{
	//for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	bIsGameClear = bIsClear;

	// 도중에 나가던지 완주했던지 모두 기록은 저장
	const float ClearTime = GetWorld()->GetTimeSeconds() - StartTime;
	const FName FieldID = GetGameInstance<UJumpGameInstance>()->GetSubsystem<UStageSystemSubsystem>()->GetChosenField();

	if (auto* Subsystem = GetGameInstance()->GetSubsystem<UStageSystemSubsystem>())
	{
		Subsystem->SaveFieldResult(FieldID, StarCount, ClearTime);
	}
	
	// 별을 모두 먹었을 때 결과 UI를 띄우기
	if (bIsClear == true)
	{
		AInGamePlayerController* PC{(Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))};
		if (PC)
		{
			PC->ShowClearUIAnimation();
		}
		OnFieldClear(FieldID);
	}
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
	OnAllClientAddedDelegate.Broadcast();

	// 로딩 UI 제거
	//MulticastRPC_RemoveLoadingUI();

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

	AInGamePlayerController* PC{(Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))};
	if (PC)
	{
		PC->ShowInGameUI();
	}
}

void AMapGameState::OnEnterField(FName FieldID)
{
	UStageSystemSubsystem* SS = GetGameInstance()->GetSubsystem<UStageSystemSubsystem>();
	if (SS->GetFieldState(FieldID) == EFieldProgressState::Cleared) return; // 이미 클리어한 맵이면 업데이트 하지말자
	SS->SetFieldState(FieldID, EFieldProgressState::InProgress);
}

void AMapGameState::OnFieldClear(FName FieldID)
{
	UStageSystemSubsystem* SS = GetGameInstance()->GetSubsystem<UStageSystemSubsystem>();
	SS->SetFieldState(FieldID, EFieldProgressState::Cleared);
}
