#include "PressedHandlerManager.h"

#include "GizmoPressedHandler.h"
#include "GizmoPrimaryPressedHandler.h"
#include "PropSlotPressedHandler.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


UPressedHandlerManager::UPressedHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

// Called when the game starts
void UPressedHandlerManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterHandler(MakeShared<FPropSlotPressedHandler>());
	RegisterHandler(MakeShared<FGizmoPrimaryPressedHandler>());
	RegisterHandler(MakeShared<FGizmoPressedHandler>());
}

void UPressedHandlerManager::InitializeComponent()
{
	Super::InitializeComponent();

	AMapEditingPawn* MapEditingPawn = Cast<AMapEditingPawn>(GetOwner());
	// MapEditingPawn->GetDragDropOperation()->OnDragged.AddDynamic(this, &UPressedHandlerManager::OnWidgetDragging);
	MapEditingPawn->GetDragDropOperation()->OnDraggedWidget.AddDynamic(this, &UPressedHandlerManager::OnWidgetDragging);
	ClickHandlerManager = MapEditingPawn->GetClickHandlerManager();
}

void UPressedHandlerManager::RegisterHandler(TSharedPtr<IPressedHandler> Handler)
{
	Handlers.Add(Handler);
	Handlers.Sort([](const TSharedPtr<IPressedHandler>& A, const TSharedPtr<IPressedHandler>& B)
	{
		return A->GetPriority() > B->GetPriority();
	});
}

bool UPressedHandlerManager::HandlePressed(FClickResponse& ControlledInfo,
	class AMapEditingPlayerController* PlayerController)
{
	for (const auto& Handler : Handlers)
	{
		if (Handler->HandlePressed(ControlledInfo, PlayerController, GizmoPressedInfo))
		{
			return true;
		}
	}
	return false;
}

void UPressedHandlerManager::InitializeSettings(FClickResponse& ControlledInfo,
	AMapEditingPlayerController* PlayerController)
{
	FVector MouseWorldPosition;
	FVector MouseDirection;
	PlayerController->DeprojectMousePositionToWorld(MouseWorldPosition, MouseDirection);

	GizmoPressedInfo.InitialMouseRayOrigin = MouseWorldPosition;
	GizmoPressedInfo.InitialMouseRayDirection = MouseDirection;
	if (!ControlledInfo.TargetProp)
	{
		return;
	}
	UGridComponent* Grid = ControlledInfo.TargetProp->GetGridComp();
	// GizmoPressedInfo.OriginalActorLocation = ControlledInfo.TargetProp->GetActorLocation() - (Grid->GetSize() * Grid->GetSnapSize() * 0.5f);
	// 교차점으로 중심을 옮김
	GizmoPressedInfo.OriginalActorLocation = ControlledInfo.TargetProp->GetActorLocation();
}

void UPressedHandlerManager::ResetPositions()
{
	GizmoPressedInfo = FGizmoPressedInfo();
}

void UPressedHandlerManager::OnWidgetDragging(FVector2D MousePosition)
{
	FClickResponse ControlledInfo = ClickHandlerManager->GetControlledClickResponse();
	AMapEditingPlayerController* PlayerController = Cast<AMapEditingPlayerController>(GetWorld()->GetFirstPlayerController());
	HandlePressed(ControlledInfo, PlayerController);
}
