// Fill out your copyright notice in the Description page of Project Settings.


#include "GizmoPrimaryPressedHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/GizmoPrimaryComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


UGizmoPrimaryPressedHandler::UGizmoPrimaryPressedHandler()
{
}

UGizmoPrimaryPressedHandler::~UGizmoPrimaryPressedHandler()
{
}

bool UGizmoPrimaryPressedHandler::HandlePressed(FClickResponse& PressedResponse,
	class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& /*GizmoPressedInfo*/)
{
	if (PressedResponse.Result != EClickHandlingResult::GizmoEditing)
	{
		return false;
	}
	
	if (!PressedResponse.TargetGizmo->IsA(UGizmoPrimaryComponent::StaticClass()))
	{
		return false;
	}

	// Snap 기능은 한 개만 지원
	if (PressedResponse.SelectedProps.Num() > 1)
	{
		return false;
	}
	
	PressedResponse.SelectedProps.Last()->SetGizmosCollision(false);
	
	FHitResult HitResult;
	bool bResult = PlayerController->OnPressedOperation(PressedType, HitResult);
	
	PressedResponse.SelectedProps.Last()->SetGizmosCollision(true);
	
	if (!bResult)
	{
		return false;
	}
	
	TWeakObjectPtr<UGridComponent> GridComponent = PressedResponse.SelectedProps.Last()->GetGridComp();
	UGridComponent* Grid = GridComponent.Get();
	if (!Grid)
	{
		return false;
	}

	Grid->MoveByGizmoPrimary(HitResult.Location, HitResult);
	
	return true;
}
