// Fill out your copyright notice in the Description page of Project Settings.


#include "BackgroundClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"


UBackgroundClickHandler::UBackgroundClickHandler()
{
}

UBackgroundClickHandler::~UBackgroundClickHandler()
{
}

bool UBackgroundClickHandler::HandleClick(FClickResponse& ClickResponse,
	class AMapEditingPlayerController* PlayerController, FClickContext& ClickContext)
{
	ClickResponse.Result = EClickHandlingResult::BackgroundEditing;

	// true가 될 경우 액터가 선택됨
	APrimitiveProp* LastSelected = FCommonUtil::SafeLast(ClickResponse.SelectedProps);
	if (PlayerController->OnClickOperation(LastSelected, ClickResponse))
	{
		ClickResponse.DebugMessage = TEXT("Background Click");

		// 기존의 Gizmo가 선택되어 있다면 UnSelected 처리
		if (UGizmoComponent* ControlledGizmo = ClickResponse.TargetGizmo)
		{
			ControlledGizmo->SetUnSelected();
			ClickResponse.TargetGizmo = nullptr;
		}

		// 기존의 액터가 선택되어 있다면 UnSelected 처리

		for (APrimitiveProp* SelectedProp : ClickResponse.SelectedProps)
		{
			if (SelectedProp)
			{
				SelectedProp->SetUnSelected();
			}
		}
		ClickResponse.SelectedProps.Empty();

		return true;
	}
	return false;
}