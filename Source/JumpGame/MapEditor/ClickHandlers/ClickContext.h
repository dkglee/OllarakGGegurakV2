#pragma once

#include "CoreMinimal.h"
#include "ClickContext.generated.h"

USTRUCT()
struct FClickContext
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 Flags = 0;
	enum : uint8
	{
		RotateGizmoMode = 1 << 0,  // 0x01
		CtrlMultiSelect = 1 << 1,  // 0x02
		ShiftMultiSelect = 1 << 2, // 0x04
	};

	FORCEINLINE bool Has(uint8 Flag) const { return (Flags & Flag) != 0; }
};
