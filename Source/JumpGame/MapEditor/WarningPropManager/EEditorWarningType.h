#pragma once

#include "EEditorWarningType.generated.h"

UENUM()
enum class EEditorWarningType : uint8
{
	None = 0			UMETA(DisplayName = "None"),
	Collision = 1		UMETA(DisplayName = "CollisionProp"), // 충돌하는 Prop
	OverCount = 2		UMETA(DisplayName = "OverNecessaryProp"), // 필수 Prop 과다
	LessCount = 3		UMETA(DisplayName = "LessNecessaryProp"), // 필수 Prop 부족
};
