#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	File UMETA(DisplayName = "File"),
	Directory UMETA(DisplayName = "Directory"),
	BackButton UMETA(DisplayName = "BackButton"),
	Unknown UMETA(DisplayName = "Unknown")
};