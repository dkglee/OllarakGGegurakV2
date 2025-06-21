// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitRoomUI.h"

#include "MapSlotUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "SelectRoomUI.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "UICam/LobbySubCamera.h"

void UWaitRoomUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());

	SubCamera = Cast<ALobbySubCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ALobbySubCamera::StaticClass()));
	
	PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetViewTarget(SubCamera);
	
	Btn_GameStart->OnClicked.AddDynamic(this, &UWaitRoomUI::OnClickGameStart);
	// TODO : 맵 선택 하는거 더 자연스럽게
	Btn_SelectMap->OnClicked.AddDynamic(this, &UWaitRoomUI::OnClickSelectMap);
	Btn_BackFromLobby->OnClicked.AddDynamic(this, &UWaitRoomUI::OnClickBackFromLobby);

	SelectRoomUI = CreateWidget<USelectRoomUI>(GetWorld(), SelectRoomFactory);
	if (SelectRoomUI)
	{
		SelectRoomUI->OnMapSelectedDelegate.BindDynamic(this, &UWaitRoomUI::OnMapSelected);
	}

	Text_MapName->SetText(FText::FromString(TEXT("No Map")));

	DefaultTintColor = Image_Selected->GetBrush().TintColor;
}

void UWaitRoomUI::OnMapSelected(UMapSlotUI* MapSlotUI)
{
	if (!MapSlotUI) return;

	ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (!LobbyGameState) return;

	TArray<uint8> ImageData = MapSlotUI->ImageData;
	const int32 ImageSize = ImageData.Num();
	const int32 ChunkSize = 8192; // 8KB
	
	LobbyGameState->MulticastRPC_ClientBeginRecvImage(MapSlotUI->GetMapName(), ImageSize);
	for (int32 Offset = 0; Offset < ImageSize; Offset += ChunkSize)
	{
		const int32 Size = FMath::Min(ChunkSize, ImageSize - Offset);

		// 임시 배열에 잘라 담는다
		TArray<uint8> Chunk;
		Chunk.Append(&ImageData[Offset], Size);

		// Reliable 전송
		LobbyGameState->MulticastRPC_ClientRecvImageChunk(Chunk, Offset);
	}
	LobbyGameState->MulticastRPC_ClientEndRecvImage();
}

void UWaitRoomUI::UpdateCurrentMapThumbnail(UTexture2D* Texture)
{
	if (Texture)
	{
		Image_Selected->SetBrushFromTexture(Texture);
		Image_Selected->SetBrushTintColor(FLinearColor::White);
	}
	else
	{
		Image_Selected->SetBrushFromTexture(nullptr);
		Image_Selected->SetBrushTintColor(DefaultTintColor);
	}

	if (IsValid(CurrentMapThumbnail))
	{
		CurrentMapThumbnail->MarkAsGarbage();
	}
	CurrentMapThumbnail = Texture;
}

void UWaitRoomUI::UpdateCurrentMapName(const FString& MapName)
{
	Text_MapName->SetText(FText::FromString(MapName));
}

void UWaitRoomUI::OnClickGameStart()
{
	// 버튼 비활성화
	Btn_GameStart->SetIsEnabled(false);
	Btn_SelectMap->SetIsEnabled(false);
	Btn_BackFromLobby->SetIsEnabled(false);
	
	// 서버만 게임을 시작할 수 있다
	PC = GetWorld()->GetFirstPlayerController();
	if (PC->HasAuthority())
	{
		int32 CurrentPlayer = 0;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			PC = Iterator->Get();
			if (PC)
			{
				CurrentPlayer++;
			}
		}
		GI->SetMaxPlayer(CurrentPlayer);
		// TODO: 선택한 게임레벨로 이동하게하자
		GetWorld()->ServerTravel(TEXT("/Game/Maps/InGameLevel?listen"));
		// GetWorld()->ServerTravel(TEXT("/Game/Maps/TestInGameLevel?listen"));
		UE_LOG(LogTemp, Warning, TEXT("게임시작"));
	}
}

void UWaitRoomUI::OnClickSelectMap()
{
	// 서버만 맵을 고를 수 있다
	PC = GetWorld()->GetFirstPlayerController();
	if (PC->HasAuthority())
	{
		// 맵선택 UI가 뜨게 하자
		SelectRoomUI->AddToViewport();
		SelectRoomUI->InitSelectRoomUI();
		ShowMapSelect();
		FFastLogger::LogConsole(TEXT("OnClickSelectMapOnClickSelectMapOnClickSelectMap"));
	}
}

void UWaitRoomUI::OnClickBackFromLobby()
{
	// 세션파괴해야됨!
	GI->LeaveSession(true);
}

void UWaitRoomUI::UpdateWaitPlayer()
{
	if (!GI) return;

	// 이미지 배열
	TArray<UImage*> UpdateImgs = { Image_user1, Image_user2, Image_user3, Image_user4, Image_user5, Image_user6};
	// 플레이어 이름 배열
	TArray<UTextBlock*> UpdateNames = { Text_Player1, Text_Player2, Text_Player3, Text_Player4, Text_Player5, Text_Player6};
	
	TMap<FString, FPlayerInfo> PlayerInfo = GI->GetPlayerInfo();
	
	for (int i = 0; i < UpdateImgs.Num(); i++)
	{
		for (auto& it : PlayerInfo)
		{
			if (it.Value.PlayerID == i)
			{
				// 플레이어 인덱스와 같은 이미지, 이름을 보이게 하자
				UpdateImgs[i]->SetRenderOpacity(1);
				UpdateNames[i]->SetText(FText::FromString(it.Value.PlayerName));
				UpdateNames[i]->SetRenderOpacity(1);
			}
		}
	}
}

void UWaitRoomUI::ShowMapSelect_Implementation()
{
	
}

