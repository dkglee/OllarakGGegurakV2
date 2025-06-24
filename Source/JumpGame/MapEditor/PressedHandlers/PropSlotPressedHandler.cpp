// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSlotPressedHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


UPropSlotPressedHandler::UPropSlotPressedHandler()
{
}

UPropSlotPressedHandler::~UPropSlotPressedHandler()
{
}

bool UPropSlotPressedHandler::HandlePressed(FClickResponse& PressedResponse,
	class AMapEditingPlayerController* PlayerController, const FGizmoPressedInfo& /*GizmoPressedInfo*/)
{
	if (PressedResponse.Result != EClickHandlingResult::UIEditing)
	{
		return false;
	}
	if (!FCommonUtil::SafeLast(PressedResponse.SelectedProps))
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