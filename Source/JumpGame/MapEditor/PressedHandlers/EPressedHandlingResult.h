#pragma once

#include "CoreMinimal.h"
#include "EPressedHandlingResult.generated.h"

UENUM(BlueprintType)
enum class EPressedHandlingResult : uint8
{
	None           UMETA(DisplayName = "None"),           // 아무것도 처리되지 않음
	GizmoPrimaryPressed   UMETA(DisplayName = "Gizmo Pimary Handling"),  // Primary Gizmo 조작 처리됨
	GizmoPressed   UMETA(DisplayName = "Gizmo Handling"),  // Gizmo 조작 처리됨
	WidgetPressed  UMETA(DisplayName = "Widget Handling"), // Widget 조작 처리됨
};