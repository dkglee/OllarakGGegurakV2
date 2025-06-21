// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetMapEditDragDropOperation.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Widget.h"
#include "JumpGame/Utils/CursorManager.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Slate/SceneViewport.h"

UWidgetMapEditDragDropOperation::UWidgetMapEditDragDropOperation()
{
	OnDragged.AddDynamic(this, &UWidgetMapEditDragDropOperation::OnDragging);
	OnDragCancelled.AddDynamic(this, &UWidgetMapEditDragDropOperation::OnDraggingCancelled);
}

void UWidgetMapEditDragDropOperation::BeginDestroy()
{
	Super::BeginDestroy();
}

void UWidgetMapEditDragDropOperation::Dragged_Implementation(const FPointerEvent& PointerEvent)
{
	Super::Dragged_Implementation(PointerEvent);

	FVector2D LocalPx, LocalUMG;
	if (ScreenToViewportLocal(PointerEvent, GetWorld(), LocalPx, LocalUMG))
	{
		// UMG 좌표(= Blueprint GetMousePositionOnViewport()와 동일)
		OnDraggedWidget.Broadcast(LocalPx);

		// 필요하다면 로그로 확인
		// FFastLogger::LogScreen(FColor::Emerald,
		// 	TEXT("Dragged (VP px): %.1f, %.1f  |  UMG: %.1f, %.1f"),
		// 	LocalPx.X, LocalPx.Y, LocalUMG.X, LocalUMG.Y);
	}
}

void UWidgetMapEditDragDropOperation::OnDragging(UDragDropOperation* Operation)
{
	// FFastLogger::LogScreen(FColor::Emerald, TEXT("Dragging"));
	UCursorManager::SetCursor(this, ECursorName::LeafPoint);
}

void UWidgetMapEditDragDropOperation::OnDraggingCancelled(UDragDropOperation* Operation)
{
	OnDragCancelledWidget.Broadcast();
	UCursorManager::SetCursor(this, ECursorName::LeafCursor);
}

void UWidgetMapEditDragDropOperation::OnDragEnter()
{
	if (DefaultDragVisual)
	{
		DefaultDragVisual->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWidgetMapEditDragDropOperation::OnDragLeave()
{
	if (DefaultDragVisual)
	{
		DefaultDragVisual->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UWidgetMapEditDragDropOperation::ScreenToViewportLocal(const FPointerEvent& PointerEvent, UWorld* World,
	FVector2D& OutLocalPx, FVector2D& OutLocalUMG)
{
	// 0. 필수 객체 확인
	if (!GEngine || !GEngine->GameViewport)
		return false;

	FSceneViewport* SceneVP = GEngine->GameViewport->GetGameViewport();
	if (!SceneVP)
		return false;

	// 1. 스크린 픽셀 좌표
	const FVector2D ScreenPos = PointerEvent.GetScreenSpacePosition();

	// 2. '가상 데스크톱 픽셀' → 뷰포트 정규화(0~1)
	const FVector2D Norm =
		SceneVP->VirtualDesktopPixelToViewport(
			FIntPoint(FMath::RoundToInt(ScreenPos.X),
					  FMath::RoundToInt(ScreenPos.Y)));

	// 3. 정규화 → 뷰포트 내부 픽셀
	const FIntPoint VPSize = SceneVP->GetSizeXY();
	OutLocalPx = Norm * FVector2D(VPSize);

	// 4. DPI 스케일을 나누면 UMG 좌표가 된다
	const float Scale =
		World ? UWidgetLayoutLibrary::GetViewportScale(World) : 1.f;
	OutLocalUMG = OutLocalPx / Scale;

	return true;
}
