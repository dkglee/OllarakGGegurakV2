// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameUI.h"

#include "MapSlotUI.h"
#include "SelectRoomUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/ClientRoomGameState.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Maps/Node/NodeTutorial.h"
#include "Kismet/GameplayStatics.h"
#include "UICam/LobbyCameraComp.h"

void UCustomGameUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	
	ALobbyFrog* Frog = Cast<ALobbyFrog>(UGameplayStatics::GetActorOfClass(GetWorld(), ALobbyFrog::StaticClass()));
	CameraComp = Cast<ULobbyCameraComp>(Frog->CameraComp);
	
	Btn_Back->OnClicked.AddDynamic(this, &UCustomGameUI::OnClickBtn_Back);
	Btn_GameStart->OnClicked.AddDynamic(this, &UCustomGameUI::OnClickGameStart);
	Btn_SelectMap->OnClicked.AddDynamic(this, &UCustomGameUI::OnClickSelectMap);

	SelectRoomUI = CreateWidget<USelectRoomUI>(GetWorld(), SelectRoomFactory);
	if (SelectRoomUI)
	{
		SelectRoomUI->OnMapSelectedDelegate.BindDynamic(this, &UCustomGameUI::OnMapSelected);
	}
	Text_MapName->SetText(FText::FromString(TEXT("No Map")));
	DefaultTintColor = Image_Selected->GetBrush().TintColor;
}

void UCustomGameUI::OnClickBtn_Back()
{
	CameraComp->SetViewTarget(ECameraState::Main);
	OnClickBackToLobby.Broadcast();
	RemoveFromParent();
}

void UCustomGameUI::OnClickGameStart()
{
	// 버튼 비활성화
	Btn_GameStart->SetIsEnabled(false);
	Btn_SelectMap->SetIsEnabled(false);
	Btn_Back->SetIsEnabled(false);
	
	// TODO: 선택한 게임레벨로 이동하게하자
	UE_LOG(LogTemp, Warning, TEXT("게임시작"));
}

void UCustomGameUI::OnMapSelected(class UMapSlotUI* MapSlotUI)
{
	if (!MapSlotUI) return;
	
	AClientRoomGameState* ClientRoomGameState = Cast<AClientRoomGameState>(GetWorld()->GetGameState());
	if (!ClientRoomGameState) return;

	TArray<uint8> ImageData = MapSlotUI->ImageData;
	const int32 ImageSize = ImageData.Num();
	const int32 ChunkSize = 8192; // 8KB
	
	ClientRoomGameState->MulticastRPC_ClientBeginRecvImage(MapSlotUI->GetMapName(), ImageSize);
	for (int32 Offset = 0; Offset < ImageSize; Offset += ChunkSize)
	{
		const int32 Size = FMath::Min(ChunkSize, ImageSize - Offset);

		// 임시 배열에 잘라 담는다
		TArray<uint8> Chunk;
		Chunk.Append(&ImageData[Offset], Size);

		// Reliable 전송
		ClientRoomGameState->MulticastRPC_ClientRecvImageChunk(Chunk, Offset);
	}
	ClientRoomGameState->MulticastRPC_ClientEndRecvImage();
}

void UCustomGameUI::UpdateCurrentMapThumbnail(UTexture2D* Texture)
{
	FFastLogger::LogConsole(TEXT("Loading Map Thumbnail..."));
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

	FFastLogger::LogConsole(TEXT("UpdateCurrentMapThumbnail"));
}

void UCustomGameUI::UpdateCurrentMapName(const FString& MapName)
{
	Text_MapName->SetText(FText::FromString(MapName));
	FFastLogger::LogConsole(TEXT("UpdateCurrentMapName: %s"), *MapName);
}

void UCustomGameUI::OnClickSelectMap()
{
	// 맵선택 UI가 뜨게 하자
	SelectRoomUI->AddToViewport();
	SelectRoomUI->InitSelectRoomUI();
	ShowMapSelect();
}

void UCustomGameUI::ShowMapSelect_Implementation()
{
}
