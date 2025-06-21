// Fill out your copyright notice in the Description page of Project Settings.


#include "CursorManager.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void UCursorManager::SetCursor(UObject* WorldContextObject, ECursorName CursorType)
{
	EMouseCursor::Type Type = EMouseCursor::Default;
	FName Name = FName();
	FVector2D HotSpot = FVector2D::ZeroVector;
	float PixelSize{32.f};
	switch (CursorType)
	{
	case ECursorName::GreenCursor:
		Name = FName("GreenCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::FlyCursor:
		Name = FName("FlyCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::BeeCursor:
		Name = FName("BeeCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::BeePoint:
		Name = FName("BeePoint");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::CartoonFrogCursor:
		Name = FName("CartoonFrogCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::CartoonFrogPoint:
		Name = FName("CartoonFrogPoint");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::GreenPixelCursor:
		Name = FName("GreenPixelCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::GreenPixelPoint:
		Name = FName("GreenPixelPoint");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::LeafCursor:
		Name = FName("LeafCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::LeafPoint:
		Name = FName("LeafPoint");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::MintPoint:
		Name = FName("MintPoint");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::MintCursor:
		Name = FName("MintCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::SadFrogPoint:
		Name = FName("SadFrogPoint");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::SadFrogCursor:
		Name = FName("SadFrogCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::UmbrellaPoint:
		Name = FName("UmbrellaPoint");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	case ECursorName::UmbrellaCursor:
		Name = FName("UmbrellaCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	default:
		Name = FName("GreenCursor");
		HotSpot = FVector2D(1.f / PixelSize, 1.f / PixelSize);
		break;
	}

	UWidgetBlueprintLibrary::SetHardwareCursor(WorldContextObject, Type, Name, HotSpot);
}
