// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientRoomGameState.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "JumpGame/Maps/Node/StageMapNodeComponent.h"
#include "JumpGame/Maps/Node/StageOutSign.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/StageSystem/StageSystemSubsystem.h"
#include "JumpGame/UI/ClientRoomUI.h"
#include "JumpGame/UI/CustomGameUI.h"
#include "JumpGame/UI/FriendsList.h"
#include "JumpGame/UI/Lobby/Follower.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class ALobbyPlayerController;

AClientRoomGameState::AClientRoomGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	PrevLoc = FVector2D::ZeroVector;

	ConstructorHelpers::FClassFinder<UClientRoomUI> TempClientUI(TEXT("/Game/UI/LobbyUI/WBP_ClientRoomUI.WBP_ClientRoomUI_C"));
	if (TempClientUI.Succeeded())
	{
		ClientRoomUIClass = TempClientUI.Class;
	}

	LoadMapComponent = CreateDefaultSubobject<ULoadMapComponent>(TEXT("LoadMapComponent"));
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

	// 위젯 생성
	if (ClientRoomUIClass)
	{
		ClientRoomUI = CreateWidget<UClientRoomUI>(GetWorld(), ClientRoomUIClass);
		if (ClientRoomUI)
		{
			ClientRoomUI->AddToViewport();
		}
	}

	const TCHAR* Value =
	GetWorld()->URL.GetOption(TEXT("AutoStartStage="), /*Default*/ nullptr);
	if (Value && ClientRoomUI)
	{
		ClientRoomUI->OnClickGoStartStageGame();
	}
	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetGameInstance());
	if (!GI) return;
	GI->bCustomGameMode = false;
	
	UStageSystemSubsystem* SGI = GetWorld()->GetGameInstance()->GetSubsystem<UStageSystemSubsystem>();
	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(),ALobbyFrog::StaticClass()));
	if (!SGI->bNodeRestore) return;

	Frog->RestoreNodePosition();
	Frog->StageMapNodeComponent->StageOutSign->SpawnSign(false);
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
	// ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	// if (PC)
	// {
	// 	UFriendsList* FriendList = PC->BottomNaviBarUI->FriendsList;
	// 	PC->OnFriendListUpdated.AddDynamic(FriendList, &UFriendsList::OnFriendListReceived);
	//
	// 	// 자동 요청
	// 	PC->RequestFriendList();
	// 	FriendList->Btn_Refresh->SetIsEnabled(false);
	// }
}

void AClientRoomGameState::MulticastRPC_ClientBeginRecvImage_Implementation(const FString& InMapName,
	int32 InImageByteSize)
{
	FFastLogger::LogConsole(TEXT("이미지1111111111"));
	CurrentMapName = InMapName;
	CurrentMapImageData.SetNumUninitialized(InImageByteSize); // 목적지 버퍼 확보
	ClientRoomUI->CustomGameUI->UpdateCurrentMapName(CurrentMapName);
	RecvBytes = 0;
}

void AClientRoomGameState::MulticastRPC_ClientRecvImageChunk_Implementation(const TArray<uint8>& InChunk,
	int32 InOffset)
{
	FFastLogger::LogConsole(TEXT("이미지2222222"));
	if (InOffset + InChunk.Num() > CurrentMapImageData.Num()) { return; }
	FFastLogger::LogConsole(TEXT("이미지333333333"));
	FMemory::Memcpy(CurrentMapImageData.GetData() + InOffset,
					InChunk.GetData(),
					InChunk.Num());

	RecvBytes += InChunk.Num();
}

void AClientRoomGameState::MulticastRPC_ClientEndRecvImage_Implementation()
{
	FFastLogger::LogConsole(TEXT("이미지44444444444"));
	if (RecvBytes != CurrentMapImageData.Num())
	{
		FFastLogger::LogConsole(TEXT("Thumbnail lost! (%d/%d)"), RecvBytes, CurrentMapImageData.Num());
		return;
	}

	// ① 바이너리 → UTexture2D
	UTexture2D* Thumbnail = MakeTextureFromBytes(CurrentMapImageData);

	// ② UI 위젯 반영
	ClientRoomUI->CustomGameUI->UpdateCurrentMapThumbnail(Thumbnail);

	// ③ 버퍼 해제
	CurrentMapImageData.Empty();
	RecvBytes = 0;
}

UTexture2D* AClientRoomGameState::MakeTextureFromBytes(const TArray<uint8>& FileData)
{
	IImageWrapperModule& ImgMod = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
	// PNG / JPEG 자동 감지
	EImageFormat Fmt = ImgMod.DetectImageFormat(FileData.GetData(), FileData.Num());
	TSharedPtr<IImageWrapper> Wrapper = ImgMod.CreateImageWrapper(Fmt);

	if (!Wrapper.IsValid() || !Wrapper->SetCompressed(FileData.GetData(), FileData.Num()))
		return nullptr;

	TArray<uint8> RawBGRA;
	if (!Wrapper->GetRaw(ERGBFormat::BGRA, 8, RawBGRA))
		return nullptr;

	const int32 W = Wrapper->GetWidth();
	const int32 H = Wrapper->GetHeight();

	UTexture2D* Texture = UTexture2D::CreateTransient(W, H, PF_B8G8R8A8);
	if (!Texture) return nullptr;

	// 텍스처에 데이터 복사
	void* MipData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(MipData, RawBGRA.GetData(), RawBGRA.Num());
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	Texture->SRGB = true;
	Texture->UpdateResource();

	return Texture;
}
