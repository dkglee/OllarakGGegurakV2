// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSlotUI.h"

#include "JsonObjectConverter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/Props/SaveLoad/SaveData.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"


void UMapSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_MapSlot->OnClicked.AddDynamic(this, &UMapSlotUI::OnBtnMapSlotClicked);
}

void UMapSlotUI::Init(const FString& InMapFullPath, const FString& InMapName)
{
	// 맵 이름에서 .json을 제거하고 이름만 가져온다
	if (InMapName.EndsWith(TEXT(".json")))
	{
		MapName = InMapName.LeftChop(5); // .json 제거
	}
	else
	{
		MapName = InMapName;
	}
	MapFullPath = InMapFullPath;
	// 파일 이름이 너무 길면 자르자
	if (Text_MapName)
	{
		FString ShortenedMapName = MapName;
		if (ShortenedMapName.Len() > 10)
		{
			ShortenedMapName = ShortenedMapName.Left(10) + TEXT("...");
		}
		Text_MapName->SetText(FText::FromString(ShortenedMapName));
	}

	// 해당 Json을 역직렬화하여 이미지 정보를 가져오고 이미지를 UImage에 씌운다.
	SavedThumbnail = GetMapThumbnail(MapFullPath);
	if (!SavedThumbnail)
	{
		return ;
	}
	FButtonStyle ButtonStyle= Btn_MapSlot->GetStyle();
	ButtonStyle.Normal.SetResourceObject(SavedThumbnail);
	ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
	ButtonStyle.Hovered.SetResourceObject(SavedThumbnail);
	ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor::White);
	ButtonStyle.Pressed.SetResourceObject(SavedThumbnail);
	ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor::White);
	Btn_MapSlot->SetStyle(ButtonStyle);
	Btn_MapSlot->SynchronizeProperties();
}

void UMapSlotUI::OnBtnMapSlotClicked()
{
	OnClicked.ExecuteIfBound(this);
	FFastLogger::LogScreen(FColor::Green, TEXT("MapSlot Clicked: %s"), *MapName);
}

UTexture2D* UMapSlotUI::GetMapThumbnail(const FString& InMapFullPath)
{
	UTexture2D* Thumbnail = nullptr;

	FSaveDataArray SaveDataArray;
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *InMapFullPath))
	{
		return Thumbnail;
	}
	if (!FJsonObjectConverter::JsonObjectStringToUStruct<FSaveDataArray>(JsonString, &SaveDataArray, 0, 0))
	{
		return Thumbnail;
	}
	
	// Base64로 인코딩된 이미지 데이터를 디코딩하여 UTexture2D로 변환
	if (!FBase64::Decode(SaveDataArray.ImageBase64, ImageData))
	{
		return Thumbnail;
	}

	// 이미지 데이터를 UTexture2D로 변환
	Thumbnail = MakeTextureFromBytes(ImageData);
	
	return Thumbnail;
}

UTexture2D* UMapSlotUI::MakeTextureFromBytes(const TArray<uint8>& FileData)
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

