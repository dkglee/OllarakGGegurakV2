#include "RotateGizmoClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/RotateGizmoComponent.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/MapEditor/RotateHandlers/RotateHandlerManager.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

URotateGizmoClickHandler::URotateGizmoClickHandler()
{
}

bool URotateGizmoClickHandler::HandleClick(FClickResponse& ClickResponse,
	class AMapEditingPlayerController* PlayerController, FClickContext& ClickContext)
{
	ClickResponse.Result = EClickHandlingResult::RotateGizmoEditing;
	
	if (!ClickContext.Has(FClickContext::RotateGizmoMode))
	{
		return false;
	}

	if (PlayerController->OnClickOperation(ClickResponse.SelectedProps.Last(), ClickResponse))
	{
		FHitResult HitResult = ClickResponse.HitResult;

		URotateGizmoComponent* RotateGizmoComponent = Cast<URotateGizmoComponent>(HitResult.GetComponent());
		if (!RotateGizmoComponent)
		{
			return false;
		}

		RotateGizmoComponent->SetSelected();
		RotateGizmoComponent->Clicked();

		AMapEditingPawn* Editor = Cast<AMapEditingPawn>(PlayerController->GetPawn());
		if (!Editor)
		{
			return false;
		}
		
		Editor->GetRotateHandlerManager()->HandleRotate(ClickResponse);
		
		return true;
	}
	
	return false;
}

URotateGizmoClickHandler::~URotateGizmoClickHandler()
{
}
