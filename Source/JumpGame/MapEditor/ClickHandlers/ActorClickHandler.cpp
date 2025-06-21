// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/MapEditor/RotateHandlers/RotateHandlerManager.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


UActorClickHandler::UActorClickHandler()
{
}

UActorClickHandler::~UActorClickHandler()
{
}

bool UActorClickHandler::HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode)
{
	ClickResponse.Result = EClickHandlingResult::ActorEditing;

	FFastLogger::LogScreen(FColor::Red, TEXT("Actor Clicked"));
	// true가 될 경우 액터가 선택됨
	if (PlayerController->OnClickOperation(ClickResponse.TargetProp, ClickResponse))
	{
		ClickResponse.DebugMessage = TEXT("Actor Click");

		FHitResult HitResult = ClickResponse.HitResult;

		// 기존의 Gizmo가 선택되어 있다면 UnSelected 처리
		if (UGizmoComponent* ControlledGizmo = ClickResponse.TargetGizmo)
		{
			ControlledGizmo->SetUnSelected();
			ClickResponse.TargetGizmo = nullptr;
		}

		APrimitiveProp* Temp = ClickResponse.TargetProp;
		// 기존의 액터가 선택되어 있다면 UnSelected 처리
		if (APrimitiveProp* ControlledActor = ClickResponse.TargetProp)
		{
			ControlledActor->SetUnSelected();
			ClickResponse.TargetProp = nullptr;
		}

		// 새로운 액터가 선택되었다면 Selected 처리
		APrimitiveProp* Actor = Cast<APrimitiveProp>(HitResult.GetActor());
		if (Actor != Temp)
		{
			// 회전 초기화
			AMapEditingPawn* ControlledActor = Cast<AMapEditingPawn>(PlayerController->GetPawn());
			if (ControlledActor)
			{
				ControlledActor->GetRotateHandlerManager()->ResetAxis();
			}
		}
		if (Actor && Actor->IsClickable())
		{
			Actor->SetSelected(bRotateGizmoMode);
			ClickResponse.TargetProp = Actor;
		}
		
		return true;
	}
	return false;
}
