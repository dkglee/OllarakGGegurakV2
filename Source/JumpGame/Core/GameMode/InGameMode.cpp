// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMode.h"

#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/Core/PlayerController/InGamePlayerController.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"

AInGameMode::AInGameMode()
{
	PlayerControllerClass = AInGamePlayerController::StaticClass();

	LoadMapComponent->OnMapLoaded.AddDynamic(this, &AInGameMode::OnMapLoaded);
}

void AInGameMode::OnMapLoaded()
{
	bIsMapReady = true;

	FFastLogger::LogConsole(TEXT("AInGameMode::OnMapLoaded"));
	for (AController* Controller : PendingPlayers)
	{
		AActor* Actor = ChoosePlayerStart(Controller);
		FFastLogger::LogConsole(TEXT("AInGameMode::OnMapLoaded : %p"), Controller);
		RestartPlayerAtPlayerStart(Controller, Actor); // 다시 스폰 시도
	}
	
	PendingPlayers.Empty();

	AMapGameState* GS{Cast<AMapGameState>(GetWorld()->GetGameState())};
	if (!GS) return;

	GS->RemoveLoadingUI();
}

void AInGameMode::BeginPlay()
{
	Super::BeginPlay();

	UJumpGameInstance* JumpGameInstance = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	if (!JumpGameInstance)
	{
		return;
	}
	if (JumpGameInstance->bCustomGameMode == false)
	{
		JumpGameInstance->CurrentMap = EMapKind::Stage;
		UE_LOG(LogTemp, Warning, TEXT("CurrentMap: %d"), JumpGameInstance->CurrentMap);
		
		// Stage System Subsystem을 가져와서 선택된 필드 ID를 가져옵니다. 
		UStageSystemSubsystem* StageSystem = JumpGameInstance->GetSubsystem<UStageSystemSubsystem>();
		if (!StageSystem)
		{
			FFastLogger::LogConsole(TEXT("StageSystemSubsystem not found!"));
			return;
		}
		FName FieldID = StageSystem->GetChosenField();
		FString FileName = StageSystem->ConvertFiledIDToPath(FieldID);
		
		LoadMapComponent->LoadMapWithString(FileName);
	}
	else
	{
		JumpGameInstance->CurrentMap = EMapKind::CustomStage;
		FString FileName = JumpGameInstance->GetMapFilePath();
		if (FileName.IsEmpty())
		{
			FFastLogger::LogConsole(TEXT("No map file found!"));
			return;
		}
		FFastLogger::LogConsole(TEXT("Loading map: %s"), *FileName);
		LoadMapComponent->LoadMapWithString(FileName);
	}
}

void AInGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	FFastLogger::LogConsole(TEXT("AInGameMode::PostLogin : %p"), NewPlayer);
}

APawn* AInGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if (!bIsMapReady)
	{
		// 로그만 남기고 스폰 보류
		if (!PendingPlayers.Contains(NewPlayer))
		{
			PendingPlayers.Add(NewPlayer);
		}
		FFastLogger::LogConsole(TEXT("SpawnDefaultPawnFor_Implementation!!!!!!!!!!"));
	
		return nullptr;
	}

	FRotator StartRotation(ForceInit);
	StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;
	FVector StartLocation = StartSpot->GetActorLocation();

	FTransform Transform = FTransform(StartRotation, StartLocation);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, Transform, SpawnInfo);
	
	return ResultPawn;
}

void AInGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
	
	if (HasAuthority() && NewPlayer->GetPawn())
	{
		AFrog* Frog = Cast<AFrog>(NewPlayer->GetPawn());
		if (Frog)
		{
			// 서버에서 조종하는 첫 번째 플레이어는 항상 0번 스킨 사용
			if (NewPlayer->IsLocalController()) // 서버가 직접 조종하는 경우
			{
				FFastLogger::LogConsole(TEXT("서버 플레이어 - 스킨 0번"));
				Frog->ServerRPC_SetSkin(0);
				return;
			}

			// 클라이언트용 플레이어들만 랜덤 스킨 배정
			if (AvailableSkinIndices.IsEmpty())
			{
				FFastLogger::LogConsole(TEXT("스킨 풀 초기화"));
				AvailableSkinIndices = {1, 2, 3, 4, 5, 6, 7, 8, 9};

				int32 LastIndex = AvailableSkinIndices.Num() - 1;
				for (int32 i = 0; i <= LastIndex; ++i)
				{
					int32 Index = FMath::RandRange(i, LastIndex);
					if (i != Index)
					{
						AvailableSkinIndices.Swap(i, Index);
					}
				}
			}

			if (!AvailableSkinIndices.IsEmpty())
			{
				int32 SelectedIndex = AvailableSkinIndices.Pop();
				FFastLogger::LogConsole(TEXT("클라이언트용 스킨 배정: %d"), SelectedIndex);
				Frog->ServerRPC_SetSkin(SelectedIndex);
			}
			else
			{
				FFastLogger::LogConsole(TEXT("스킨 부족 - 기본값 0번 사용"));
				Frog->ServerRPC_SetSkin(0); // fallback
			}
		}
	}
}
