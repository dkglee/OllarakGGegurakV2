#pragma once

#include "CoreMinimal.h"
#include "AxisEnum.generated.h"

UENUM(BlueprintType)
enum class EAxisEnum : uint8
{
	X = 0	 UMETA(DisplayName = "X"),
	Y = 1	 UMETA(DisplayName = "Y"),
	Z = 2	 UMETA(DisplayName = "Z")
};