#include "PropSlotClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/MapEditor/RotateHandlers/RotateHandlerManager.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "Kismet/GameplayStatics.h"


UPropSlotClickHandler::UPropSlotClickHandler()
{
}

UPropSlotClickHandler::~UPropSlotClickHandler()
{
}

bool UPropSlotClickHandler::HandleClick(FClickResponse& ClickResponse,
	class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode)
{
	if (ClickResponse.Result != EClickHandlingResult::UIEditing)
	{
		return false;
	}
	if (ClickResponse.ClickedPropByWidget == nullptr)
	{
		return false;
	}

	// 기존의 Gizmo가 선택되어 있다면 UnSelected 처리
	if (UGizmoComponent* ControlledGizmo = ClickResponse.TargetGizmo)
	{
		ControlledGizmo->SetUnSelected();
		ClickResponse.TargetGizmo = nullptr;
	}

	// 기존의 액터가 선택되어 있다면 UnSelected 처리
	if (APrimitiveProp* ControlledActor = ClickResponse.TargetProp)
	{
		ControlledActor->SetUnSelected();
		ClickResponse.TargetProp = nullptr;
	}

	if (EditingPawn)
	{
		EditingPawn->GetRotateHandlerManager()->ResetAxis();
	}
	ClickResponse.TargetProp = ClickResponse.ClickedPropByWidget;
	ClickResponse.TargetProp->SetSelected(bRotateGizmoMode);
	
	return true;
}
