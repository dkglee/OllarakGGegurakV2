// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFinishProp.h"

#include <filesystem>

#include "DefeatPlatform.h"
#include "RisingWaterProp.h"
#include "VictoryPlatform.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/UI/GameProgressBarUI.h"
#include "JumpGame/Utils/FastLogger.h"
#include "OnlineSubsystem.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "JumpGame/UI/InGameSettingUI.h"
#include "JumpGame/UI/WinnerPlayerUI.h"
#include "JumpGame/UI/Character/EmotionUI.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"


// Sets default values
AGameFinishProp::AGameFinishProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("GameFinish");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh
	(TEXT("/Game/Fab/LowPolySeparate/checkpoint.checkpoint"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Super::SetSize(FVector(1,1,2));

	PropDataComponent->SetPropID(TEXT("3002"));

	// 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TempWinnerEffect
	(TEXT("/Game/_Resource/FX/VFX_Toolkit_V1/ParticleSystems/356Days/Par_CloverField_01.Par_CloverField_01"));
	if (TempWinnerEffect.Succeeded())
	{
		WinnerEffect = Cast<UParticleSystem>(TempWinnerEffect.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TempVictoryPlatformEffect
	(TEXT("/Game/_Resource/FX/VFX_Toolkit_V1/ParticleSystems/356Days/Par_ClearEffect.Par_ClearEffect"));
	if (TempVictoryPlatformEffect.Succeeded())
	{
		VictoryPlatformEffect = Cast<UParticleSystem>(TempVictoryPlatformEffect.Object);
	}
}

void AGameFinishProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Frog")) return;
	if (bIsActive == false) return;
		
	Character = Cast<AFrog>(OtherActor);
	if (!bWinnerFound)
	{
		bWinnerFound = true;
		
		// 1등 캐릭터 저장
		WinnerCharacter = Character;

		// 1등 캐릭터에게 Clear UI 띄우자
		MulticastRPC_ShowClearUI();
		
		// 5초 후에 게임을 끝내자
		GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &AGameFinishProp::GameEnd, 5.0f, false);
	}
}

// Called when the game starts or when spawned
void AGameFinishProp::BeginPlay()
{
	Super::BeginPlay();
	
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	Character = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!GetWorld()->GetMapName().Contains(TEXT("InGameLevel")))
	{
		bIsActive = false;
	}

	WinnerPlayerUI = CreateWidget<UWinnerPlayerUI>(GetWorld(), WinnerPlayerUIClass);
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGameFinishProp::OnMyBeginOverlap);
}

void AGameFinishProp::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameFinishProp, WinnerCharacter);
	DOREPLIFETIME(AGameFinishProp, VictoryP);
}

void AGameFinishProp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(EffectTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AGameFinishProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameFinishProp::GameEnd()
{
	// 시상대 스폰 (박스 위치부터 위로)
	FVector VictoryPos = MeshComp->GetComponentLocation() + FVector(0,0,20000);
	FVector DefeatPos = MeshComp->GetComponentLocation() + FVector(0,0,10000);
	VictoryP = GetWorld()->SpawnActor<AVictoryPlatform>(VictoryPos, FRotator::ZeroRotator);
	DefeatP = GetWorld()->SpawnActor<ADefeatPlatform>(DefeatPos, FRotator::ZeroRotator);

	// 우승자 앞에 보게 정렬하기
	WinnerForward = VictoryP->VictoryArrow->GetForwardVector();
	WinnerRotation = WinnerForward.Rotation();
	WinnerCharacter->SetActorRotation(WinnerRotation);
	WinnerCharacter->EmotionState = EEmotionState::PlayingWinnerEmotion;
	
	// 플레이어 텔레포트
	WinnerCharacter->SetActorLocation(VictoryP->SpawnVictoryCharacter());
	if (IsValid(Character) && Character != WinnerCharacter)
	{
		Character->SetActorLocation(DefeatP->SpawnDefeatCharacter());
	}
	
	// 게임 끝 멀티캐스트
	MulticastRPC_GameEnd();

	// 그리고 진행도 UI 지우자
	AMapGameState* GS = Cast<AMapGameState>(GetWorld()->GetGameState());
	GS->MulticastRPC_RemoveProgressBarUI();

	// 물 멈추자
	ARisingWaterProp* RisingWaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));
	if (RisingWaterProp)
	{
		RisingWaterProp->StopRising();
	}
	
	// TODO: 게임 종료 처리 - 게임 한판 진행 시간 멈추기 (게임모드?)
}

void AGameFinishProp::MulticastRPC_ShowClearUI_Implementation()
{
	// 내 캐릭터가 위너인지 판단하자
	AFrog* LocalCharacter = Cast<AFrog>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (LocalCharacter == WinnerCharacter)
	{
		if (WinnerPlayerUI)
		{
			WinnerPlayerUI->AddToViewport();
			WinnerPlayerUI->PlayWinnerAnim();
		}
		LocalCharacter->SetJumpGaugeVisibility(false);
		FVector SpawnLocation = WinnerCharacter->GetActorLocation() + FVector(0,0,-75);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WinnerEffect, SpawnLocation, FRotator::ZeroRotator, FVector(1.f), true);
	}
}

void AGameFinishProp::MulticastRPC_GameEnd_Implementation()
{
	// UI 있으면 지우기
	AMapGameState* GS = Cast<AMapGameState>(GetWorld()->GetGameState());
	if (GS && GS->InGameSettingUI)
	{
		GS->InGameSettingUI->RemoveFromParent();
	}
	if (WinnerPlayerUI)
	{
		WinnerPlayerUI->RemoveFromParent();
	}
	
	// 플레이어들 조작막기
	AFrog* LocalCharacter = Cast<AFrog>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	LocalCharacter->StopMovementAndResetRotation(FRotator::ZeroRotator);
	LocalCharacter->SetFrogVignetteIntensity_PP(0);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (LocalCharacter->EmotionUI)
	{
		LocalCharacter->EmotionUI->RemoveFromParent();
	}
	
	// UI모드로 바꾸자
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
	
	// 우승자 앞에 보게 정렬하기
	WinnerCharacter->SetActorRotation(WinnerRotation);
	// 카메라 정렬
	PC->SetViewTargetWithBlend(VictoryP, 0.5f);
	// 연출
	VictoryP->DropProps();
	WinnerCharacter->PlayAnimMontage(WinnerCharacter->WinnerMontage);
	StartWinnerEffect();
	
	// UI를 띄우자
	FString Key = WinnerCharacter->GetName();
	FString WinnerName = Key; // 스팀 없을때 기본값

	// 1등 플레이어의 고유한 네트워크 ID값 가져오기
	const FUniqueNetIdRepl& NetIdRepl = WinnerCharacter->GetPlayerState<APlayerState>()->GetUniqueId();
	if (!NetIdRepl.IsValid()) return;
	
	TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
	if (!NetId.IsValid()) return;
	
	// Steam 닉네임이 있다면 사용
	if (IOnlineSubsystem* Subsys = IOnlineSubsystem::Get())
	{
		IOnlineIdentityPtr Identity = Subsys->GetIdentityInterface();
		if (Identity.IsValid())
		{
			WinnerName = Identity->GetPlayerNickname(*NetId);
		}
	}
	
	VictoryPageUI = CreateWidget<UVictoryPageUI>(GetWorld(), VictoryPageUIClass);
	if (VictoryPageUI)
	{
		VictoryPageUI->SetVictoryPlayerName(WinnerName);
		VictoryPageUI->AddToViewport();
		VictoryPageUI->PlayWinnerPageAnim();
	}
}

void AGameFinishProp::StartWinnerEffect()
{
	// 이미 실행중이면 무시하자
	if (GetWorld()->GetTimerManager().IsTimerActive(EffectTimerHandle)) return;
	
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AGameFinishProp::PlayWinnerEffect, EffectInterval, true);
}

void AGameFinishProp::PlayWinnerEffect()
{
	if (!WinnerCharacter || !VictoryPlatformEffect) return;
	
	// 랜덤 박스 범위: X: -BoxExtent.X ~ +BoxExtent.X
	FVector BoxExtent(EffectSpawnRadius, EffectSpawnRadius, 0.f); // Z는 고정
	FVector Offset;
	Offset.X = FMath::RandRange(-BoxExtent.X, BoxExtent.X);
	Offset.Y = FMath::RandRange(-BoxExtent.Y, BoxExtent.Y);
	Offset.Z = FMath::RandRange(50, 100);

	FVector SpawnLocation = WinnerCharacter->GetActorLocation() + Offset;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VictoryPlatformEffect, SpawnLocation, FRotator::ZeroRotator, FVector(0.7f), true);
}

