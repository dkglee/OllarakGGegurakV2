// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSlotPressedHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


FPropSlotPressedHandler::FPropSlotPressedHandler()
{
}

FPropSlotPressedHandler::~FPropSlotPressedHandler()
{
}

bool FPropSlotPressedHandler::HandlePressed(FClickResponse& PressedResponse,
	class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& /*GizmoPressedInfo*/)
{
	if (PressedResponse.Result != EClickHandlingResult::UIEditing)
	{
		return false;
	}
	if (!PressedResponse.TargetProp)
	{
		return false;
	}
	
	PressedResponse.TargetProp->SetGizmosCollision(false);
	
	FHitResult HitResult;
	bool bResult = PlayerController->OnPressedOperation(PressedType, HitResult);
	
	PressedResponse.TargetProp->SetGizmosCollision(true);
	
	if (!bResult)
	{
		return false;
	}
	
	TWeakObjectPtr<UGridComponent> GridComponent = PressedResponse.TargetProp->GetGridComp();
	UGridComponent* Grid = GridComponent.Get();
	if (!Grid)
	{
		return false;
	}
	
	Grid->MoveByGizmoPrimary(HitResult.Location, HitResult);
	
	return true;
}