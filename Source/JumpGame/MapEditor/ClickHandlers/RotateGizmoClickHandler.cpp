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
	class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode)
{
	ClickResponse.Result = EClickHandlingResult::RotateGizmoEditing;
	
	if (!bRotateGizmoMode)
	{
		return false;
	}

	if (PlayerController->OnClickOperation(ClickResponse.TargetProp, ClickResponse))
	{
		FHitResult HitResult = ClickResponse.HitResult;

		ClickResponse.TargetProp = Cast<APrimitiveProp>(HitResult.GetActor());

		URotateGizmoComponent* RotateGizmoComponent = Cast<URotateGizmoComponent>(HitResult.GetComponent());
		if (!RotateGizmoComponent)
		{
			return false;
		}

		RotateGizmoComponent->SetSelected();
		RotateGizmoComponent->Clicked();

		// Rotate 할 축을 들고옴
		// FVector Direction = RotateGizmo->GetDirection();

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
