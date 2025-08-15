// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiSelectClickHandler.h"

#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

bool UMultiSelectClickHandler::HandleClick(FClickResponse& ClickResponse, AMapEditingPlayerController* PlayerController,
                                           FClickContext& ClickContext)
{
	ClickResponse.Result = EClickHandlingResult::MultiSelectEditing;

	// Ctrl 키가 눌려져 있지 않다면 false를 반환
	if (!ClickContext.Has(FClickContext::CtrlMultiSelect))
	{
		return false;
	}

	// 엑터를 클릭
	if (!PlayerController->OnClickOperation(nullptr, ClickResponse))
	{
		return false;
	}

	APrimitiveProp* ClickedActor = Cast<APrimitiveProp>(ClickResponse.HitResult.GetActor());
	if (!ClickedActor)
	{
		return false;
	}

	APrimitiveProp* LastSelected = FCommonUtil::SafeLast(ClickResponse.SelectedProps);
	if (ClickResponse.SelectedProps.Contains(ClickedActor))
	{
		// 이미 선택된 액터라면 선택 해제
		ClickedActor->SetUnSelected();
		ClickResponse.SelectedProps.Remove(ClickedActor);

		// 제거 후 LastSelected가 ClickedActor와 같다면 최신 LastSelected를 찾는다.
		if (LastSelected == ClickedActor)
		{
			APrimitiveProp* NewLastSelected = FCommonUtil::SafeLast(ClickResponse.SelectedProps);
			if (NewLastSelected)
			{
				NewLastSelected->ActivateGizmos(ClickContext.Has(FClickContext::RotateGizmoMode));
			}
		}
		
		return true;
	}
	
	if (LastSelected != ClickedActor)
	{
		// LastSelected의 기즈모들을 숨긴다.
		if (LastSelected) LastSelected->DeactivateGizmos();
		ClickedActor->SetSelected(ClickContext.Has(FClickContext::RotateGizmoMode));

		// ClickedActor를 선택된 액터 목록에 추가한다.
		if (!ClickResponse.SelectedProps.Contains(ClickedActor))
		{
			ClickResponse.SelectedProps.Add(ClickedActor);
		}
	}
	
	return true;
}
