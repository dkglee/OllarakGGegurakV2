// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Components/Button.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"
#include "JumpGame/Networks/Connection/ConnectionVerifyComponent.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/UI/BottomNaviBarUI.h"
#include "JumpGame/UI/FriendsList.h"
#include "JumpGame/UI/SelectRoomUI.h"
#include "JumpGame/UI/WaitRoomUI.h"
#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
	bReplicates = true;
	
	ConstructorHelpers::FClassFinder<UWaitRoomUI> TempWaitUI(TEXT("/Game/UI/LobbyUI/WBP_WaitRoomUI.WBP_WaitRoomUI_C"));
	if (TempWaitUI.Succeeded())
	{
		WaitRoomUIClass = TempWaitUI.Class;
	}

	LoadMapComponent = CreateDefaultSubobject<ULoadMapComponent>(TEXT("LoadMapComponent"));
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();
	
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// 최대 몇명의 플레이어가 플레이를 할지 설정하는 함수 (GI에서 정보를 들고와서 설정해준다)
	// int32 MaxPlayer = GI->GetMaxPlayerCount();
	// 근데 로비니까 8명으로 강제설정
	ConnectionVerifyComponent->InitMaxPlayerCount(8);
	
	// 위젯 생성
	if (WaitRoomUIClass)
	{
		WaitRoomUI = CreateWidget<UWaitRoomUI>(GetWorld(), WaitRoomUIClass);
		if (WaitRoomUI)
		{
			WaitRoomUI->AddToViewport();
		}
	}
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ALobbyGameState::OnClientAdded(const FString& NetID)
{
	Super::OnClientAdded(NetID);

	// 업데이트 내용 멀티캐스트 (모든 사람이 알 수 있게)
	TMap<FString, FPlayerInfo>& InfoMap = GI->GetPlayerInfo();
	for (auto& it : InfoMap)
	{
		MulticastRPC_UpdateWaitImage(it.Key, it.Value);
	}
	WaitRoomUI->OnMapSelected(WaitRoomUI->SelectRoomUI->GetCurrentSelectedMapSlotUI());
}

void ALobbyGameState::OnConnectionSucceeded()
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

void ALobbyGameState::MulticastRPC_UpdateWaitImage_Implementation(const FString& PlayerKey,
                                                                  const FPlayerInfo& PlayerInfo)
{
	if (GI && !HasAuthority())
	{
		GI->AddPlayerInfo(PlayerKey, PlayerInfo);
	}
	
	if (WaitRoomUI)
	{
		WaitRoomUI->UpdateWaitPlayer();
	}
}

void ALobbyGameState::MulticastRPC_ClientBeginRecvImage_Implementation(const FString& InMapName, int32 InImageByteSize)
{
	CurrentMapName = InMapName;
	CurrentMapImageData.SetNumUninitialized(InImageByteSize); // 목적지 버퍼 확보
	WaitRoomUI->UpdateCurrentMapName(CurrentMapName);
	RecvBytes = 0;
}

void ALobbyGameState::MulticastRPC_ClientRecvImageChunk_Implementation(const TArray<uint8>& InChunk, int32 InOffset)
{
	if (InOffset + InChunk.Num() > CurrentMapImageData.Num()) { return; }

	FMemory::Memcpy(CurrentMapImageData.GetData() + InOffset,
					InChunk.GetData(),
					InChunk.Num());

	RecvBytes += InChunk.Num();
}

void ALobbyGameState::MulticastRPC_ClientEndRecvImage_Implementation()
{
	if (RecvBytes != CurrentMapImageData.Num())
	{
		FFastLogger::LogConsole(TEXT("Thumbnail lost! (%d/%d)"), RecvBytes, CurrentMapImageData.Num());
		return;
	}

	// ① 바이너리 → UTexture2D
	UTexture2D* Thumbnail = MakeTextureFromBytes(CurrentMapImageData);

	// ② UI 위젯 반영
	WaitRoomUI->UpdateCurrentMapThumbnail(Thumbnail);

	// ③ 버퍼 해제
	CurrentMapImageData.Empty();
	RecvBytes = 0;
}

UTexture2D* ALobbyGameState::MakeTextureFromBytes(const TArray<uint8>& FileData)
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

