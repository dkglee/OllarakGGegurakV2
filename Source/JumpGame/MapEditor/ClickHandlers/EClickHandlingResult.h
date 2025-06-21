#pragma once

#include "CoreMinimal.h"
#include "EClickHandlingResult.generated.h"

UENUM(BlueprintType)
enum class EClickHandlingResult : uint8
{
	None				UMETA(DisplayName = "None"),           // 아무것도 처리되지 않음
	UIEditing			UMETA(DisplayName = "UI Editing"),     // UI 조작 처리됨
	RotateGizmoEditing  UMETA(DisplayName = "Rotate Gizmo Editing"), // 회전 Gizmo 조작 처리됨
	GizmoEditing		UMETA(DisplayName = "Gizmo Editing"),  // Gizmo 조작 처리됨
	ActorEditing		UMETA(DisplayName = "Actor Editing"),  // Actor 조작 처리됨
	BackgroundEditing	UMETA(DisplayName = "Background"),    // 배경 클릭
};