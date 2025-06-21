// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectRoomUI.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "MapSlotUI.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "FileBrowser/FileBrowserUI.h"
#include "JumpGame/Utils/FastLogger.h"

void USelectRoomUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_GoBackWait->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickGoBackWait);
	Btn_SelectComplete->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickSelectComplete);
	
	Btn_AllMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickAllMap);
	Btn_OriginMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickOriginMap);
	Btn_CustomMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickCustomMap);
	Btn_PickFile->OnClicked.AddDynamic(this, &USelectRoomUI::OnPickCustomMap);

	ScrollBox_AllMap->ClearChildren();
	ScrollBox_OriginMap->ClearChildren();
	ScrollBox_CustomMap->ClearChildren();

	Text_MapName->SetText(FText::FromString(TEXT("No Map")));

	// InitSelectRoomUI();

	DefaultTintColor = Image_Selected->GetBrush().TintColor;
}

void USelectRoomUI::InitSelectRoomUI()
{
	// Current Selected Map Slot UI를 이미지에 띄워주기
	if (CurrentSelectedMapSlotUI)
	{
		Text_MapName->SetText(FText::FromString(CurrentSelectedMapSlotUI->GetMapName()));
		if (CurrentSelectedMapSlotUI->GetSavedThumbnail())
		{
			Image_Selected->SetBrushFromTexture(CurrentSelectedMapSlotUI->GetSavedThumbnail());
			Image_Selected->SetBrushTintColor(FLinearColor::White);
		}
		else
		{
			Image_Selected->SetBrushFromTexture(nullptr);
			Image_Selected->SetBrushTintColor(DefaultTintColor);
		}
	}
	
	if (bIsInitialized)
	{
		return ;
	}
	bIsInitialized = true;
	
	OriginMapList = GetMapList(TEXT(".json"), TEXT("OriginMap\\"));
	CustomMapList = GetMapList(TEXT(".json"), TEXT("CustomMap\\"));
	SavedMapList = GetMapList(TEXT(".json"), TEXT("../../Saved/SaveMap/"));
	CombinedMapList.Append(OriginMapList);
	CombinedMapList.Append(CustomMapList);
	CombinedMapList.Append(SavedMapList);
	CustomMapList.Append(SavedMapList);

	InitAllMap();
	InitOriginMap();
	InitCustomMap();

	for (int32 i = 0; i < HorizontalBoxCount; i++)
	{
		UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		MapSlotBoxes.Add(HorizontalBox);
	}

	OnClickAllMap();
}

void USelectRoomUI::OnClickGoBackWait()
{
	// 맵 선택 취소
	// RemoveFromParent();
	// if (PreviewSelectedMapSlotUI)
	// {
	// 	PreviewSelectedMapSlotUI->RemoveFromParent();
	// }
	PreviewSelectedMapSlotUI = nullptr;
}

void USelectRoomUI::OnClickSelectComplete()
{
	// TODO: 맵 정보 넘겨주기
	// 선택완료 됐으니 팝업창 지우자
	RemoveFromParent();
	// 현재 선택 중인 맵이 PickFile로 선택되었던 거랴면 RemoveFromParent를 해줌
	if (bCurrentByPickedFile)
	{
		CurrentSelectedMapSlotUI->RemoveFromParent();
	}
	bCurrentByPickedFile = bPreviewByPickedFile;
	CurrentSelectedMapSlotUI = PreviewSelectedMapSlotUI;
	PreviewSelectedMapSlotUI = nullptr;
	UJumpGameInstance* GameInstance = Cast<UJumpGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return ;
	}

	if (CurrentSelectedMapSlotUI)
	{
		GameInstance->SetMapFilePath(CurrentSelectedMapSlotUI->GetMapFullPath());
	}

	OnMapSelectedDelegate.ExecuteIfBound(CurrentSelectedMapSlotUI);
}

void USelectRoomUI::OnClickAllMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(0);
	FillScrollBox(ScrollBox_AllMap, CombinedMapListUI);
}

void USelectRoomUI::OnClickOriginMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(1);
	FillScrollBox(ScrollBox_OriginMap, OriginMapListUI);
}

void USelectRoomUI::OnClickCustomMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(2);
	FillScrollBox(ScrollBox_CustomMap, CustomMapListUI);
}

void USelectRoomUI::OnPickCustomMap()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC->HasAuthority())
	{
		return ;
	}
	ALobbyGameState* GameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		return ;
	}
	FString Suffix = TEXT(".json");
	GameState->LoadMapComponent->GetFileBrowserUI()->OnFileSelectedDelegate.BindUObject(this, &USelectRoomUI::OnPickFileComplete);
	GameState->LoadMapComponent->GetFileBrowserUI()->SetInfoText(TEXT("플레이할 맵을 선택해 주세요.\n선택을 원하지 않을 시 창을 닫아주세요."));
	GameState->LoadMapComponent->PickFile(Suffix, false);
}

void USelectRoomUI::OnPickFileComplete(const FString& FilePath, bool bSuccess)
{
	ALobbyGameState* GameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		return ;
	}
	GameState->LoadMapComponent->GetFileBrowserUI()->OnFileSelectedDelegate.Unbind();
	if (!bSuccess)
	{
		return ;
	}

	FFastLogger::LogScreen(FColor::Red, TEXT("here"));
	
	UMapSlotUI* MapSlot = CreateWidget<UMapSlotUI>(GetWorld(), MapSlotClass);
	FString FileName = FPaths::GetCleanFilename(FilePath);
	MapSlot->Init(FilePath, FileName);
	PreviewSelectedMapSlotUI = MapSlot;

	Text_MapName->SetText(FText::FromString(MapSlot->GetMapName()));
	if (MapSlot->GetSavedThumbnail())
	{
		Image_Selected->SetBrushFromTexture(MapSlot->GetSavedThumbnail());
		Image_Selected->SetBrushTintColor(FLinearColor::White);
	}
	else
	{
		Image_Selected->SetBrushFromTexture(nullptr);
		Image_Selected->SetBrushTintColor(DefaultTintColor);
	}
	
	bPreviewByPickedFile = true;
}


TArray<FString> USelectRoomUI::GetMapList(const FString& MapType, const FString& MapDir)
{
	TArray<FString> MapList;

	FString ExecutableDir = FPaths::ProjectDir() + TEXT("AppData/Content/Maps/");
	FString MapPath = FPaths::Combine(ExecutableDir, MapDir);
	FString AbsoluteMapPath = FPaths::ConvertRelativePathToFull(MapPath);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*AbsoluteMapPath))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Directory does not exist"));
		return MapList;
	}
	
	PlatformFile.IterateDirectory(*AbsoluteMapPath, [this, &MapList, &MapType](const TCHAR* Path, bool bIsDirectory) -> bool
	{
		const FString ItemPath = FString(Path);
		// const FString ItemName = FPaths::GetCleanFilename(ItemPath);

		if (!bIsDirectory && ItemPath.EndsWith(MapType))
		{
			MapList.Add(ItemPath);
		}
		return true;
	});
	
	return MapList;
}

void USelectRoomUI::InitAllMap()
{
	for (FString& MapFullPath : CombinedMapList)
	{
		FString MapName = FPaths::GetCleanFilename(MapFullPath);
		UMapSlotUI* MapSlot = CreateWidget<UMapSlotUI>(GetWorld(), MapSlotClass);
		MapSlot->Init(MapFullPath, MapName);
		MapSlot->OnClicked.BindUObject(this, &USelectRoomUI::OnMapClicked);
		MapSlot->SetPadding(FMargin(0, 0, 10.0f, 10.0f));
		CombinedMapListUI.Add(MapSlot);
	}
}

void USelectRoomUI::InitOriginMap()
{
	for (FString& MapFullPath : OriginMapList)
	{
		FString MapName = FPaths::GetCleanFilename(MapFullPath);
		UMapSlotUI* MapSlot = CreateWidget<UMapSlotUI>(GetWorld(), MapSlotClass);
		MapSlot->Init(MapFullPath, MapName);
		MapSlot->OnClicked.BindUObject(this, &USelectRoomUI::OnMapClicked);
		MapSlot->SetPadding(FMargin(0, 0, 10.0f, 10.0f));
		OriginMapListUI.Add(MapSlot);
	}
}

void USelectRoomUI::InitCustomMap()
{
	for (FString& MapFullPath : CustomMapList)
	{
		FString MapName = FPaths::GetCleanFilename(MapFullPath);
		UMapSlotUI* MapSlot = CreateWidget<UMapSlotUI>(GetWorld(), MapSlotClass);
		MapSlot->Init(MapFullPath, MapName);
		MapSlot->OnClicked.BindUObject(this, &USelectRoomUI::OnMapClicked);
		MapSlot->SetPadding(FMargin(0, 0, 10.0f, 10.0f));
		CustomMapListUI.Add(MapSlot);
	}
}

void USelectRoomUI::OnMapClicked(UMapSlotUI* MapSlot)
{
	UJumpGameInstance* GameInstance = Cast<UJumpGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return ;
	}

	PreviewSelectedMapSlotUI = MapSlot;

	Text_MapName->SetText(FText::FromString(MapSlot->GetMapName()));
	if (MapSlot->GetSavedThumbnail())
	{
		Image_Selected->SetBrushFromTexture(MapSlot->GetSavedThumbnail());
		Image_Selected->SetBrushTintColor(FLinearColor::White);
	}
	else
	{
		Image_Selected->SetBrushFromTexture(nullptr);
		Image_Selected->SetBrushTintColor(DefaultTintColor);
	}
	
	bPreviewByPickedFile = false;
}

void USelectRoomUI::FillScrollBox(UScrollBox* ScrollBox, const TArray<UMapSlotUI*>& MapList)
{
	// 초기화
	ScrollBox->ClearChildren();
	for (int32 i = 0; i < MapSlotBoxes.Num(); i++)
	{
		MapSlotBoxes[i]->ClearChildren();
	}

	// 각각의 HorizontalBox에 3개씩 넣어준다
	const int32 NumPerRow = 3;
	int32 RowCount = 0;
	for (int32 i = 0; i < MapList.Num(); i += NumPerRow)
	{
		UHorizontalBox* HorizontalBox = MapSlotBoxes[RowCount++]; 
		for (int32 j = 0; j < NumPerRow; j++)
		{
			int32 Index = i + j;
			if (Index >= MapList.Num())
				break;

			UMapSlotUI* InSlot = MapList[Index];
			HorizontalBox->AddChild(InSlot);
		}

		ScrollBox->AddChild(HorizontalBox);
	}
}
