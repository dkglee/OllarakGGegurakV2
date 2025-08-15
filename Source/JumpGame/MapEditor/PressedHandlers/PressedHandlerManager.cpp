#include "PressedHandlerManager.h"

#include "CopyPressedHandler.h"
#include "GizmoPressedHandler.h"
#include "GizmoPrimaryPressedHandler.h"
#include "PropSlotPressedHandler.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
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

	UPressedHandlerInterface* PropSlotPressedHandler = NewObject<UPropSlotPressedHandler>(this);
	UPressedHandlerInterface* CopyPressedHandler = NewObject<UCopyPressedHandler>(this);
	UPressedHandlerInterface* GizmoPrimaryPressedHandler = NewObject<UGizmoPrimaryPressedHandler>(this);
	UPressedHandlerInterface* GizmoPressedHandler = NewObject<UGizmoPressedHandler>(this);
	
	RegisterHandler(PropSlotPressedHandler);
	RegisterHandler(CopyPressedHandler);
	RegisterHandler(GizmoPrimaryPressedHandler);
	RegisterHandler(GizmoPressedHandler);
}

void UPressedHandlerManager::InitializeComponent()
{
	Super::InitializeComponent();

	AMapEditingPawn* MapEditingPawn = Cast<AMapEditingPawn>(GetOwner());
	// MapEditingPawn->GetDragDropOperation()->OnDragged.AddDynamic(this, &UPressedHandlerManager::OnWidgetDragging);
	MapEditingPawn->GetDragDropOperation()->OnDraggedWidget.AddDynamic(this, &UPressedHandlerManager::OnWidgetDragging);
	ClickHandlerManager = MapEditingPawn->GetClickHandlerManager();
}

void UPressedHandlerManager::RegisterHandler(UPressedHandlerInterface* Handler)
{
	Handlers.Add(Handler);

	Algo::Sort(Handlers, [](const UPressedHandlerInterface* A, const UPressedHandlerInterface* B)
	{
		return A->GetPriority() > B->GetPriority();   // 우선순위가 높은 것이 앞으로
	});
	Handler->Init(Cast<AMapEditingPawn>(GetOwner()));
}

bool UPressedHandlerManager::HandlePressed(FClickResponse& ControlledInfo,
	class AMapEditingPlayerController* PlayerController)
{
	GizmoPressedInfo.Flags |= bCopyMode ? FGizmoPressedInfo::CopyMode : 0;
	
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

	GizmoPressedInfo.OriginalActorLocations.Empty();
	
	PlayerController->DeprojectMousePositionToWorld(MouseWorldPosition, MouseDirection);

	GizmoPressedInfo.InitialMouseRayOrigin = MouseWorldPosition;
	GizmoPressedInfo.InitialMouseRayDirection = MouseDirection;
	if (ControlledInfo.SelectedProps.Num() == 0)
	{
		return;
	}
	
	for (APrimitiveProp* SelectedProp : ControlledInfo.SelectedProps)
	{
		if (SelectedProp && SelectedProp->IsValidLowLevel())
		{
			GizmoPressedInfo.OriginalActorLocations.Add(SelectedProp, SelectedProp->GetActorLocation());
		}
	}
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
