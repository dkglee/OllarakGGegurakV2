// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CursorManager.generated.h"

UENUM(BlueprintType)
enum class ECursorName : uint8
{
	GreenCursor UMETA(DisplayName = "GreenCursor"),
	FlyCursor UMETA(DisplayName = "FlyCursor"),
	BeeCursor UMETA(DisplayName = "BeeCursor"),
	BeePoint UMETA(DisplayName = "BeePoint"),
	CartoonFrogCursor UMETA(DisplayName = "CartoonFrogCursor"),
	CartoonFrogPoint UMETA(DisplayName = "CartoonFrogPoint"),
	GreenPixelCursor UMETA(DisplayName = "GreenPixelCursor"),
	GreenPixelPoint UMETA(DisplayName = "GreenPixelPoint"),
	LeafCursor UMETA(DisplayName = "LeafCursor"),
	LeafPoint UMETA(DisplayName = "LeafPoint"),
	MintPoint UMETA(DisplayName = "MintPoint"),
	MintCursor UMETA(DisplayName = "MintCursor"),
	SadFrogPoint UMETA(DisplayName = "SadFrogPoint"),
	SadFrogCursor UMETA(DisplayName = "SadFrogCursor"),
	UmbrellaPoint UMETA(DisplayName = "UmbrellaPoint"),
	UmbrellaCursor UMETA(DisplayName = "UmbrellaCursor"),
};

UCLASS()
class JUMPGAME_API UCursorManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void SetCursor(UObject* WorldContextObject, ECursorName CursorType);
};
