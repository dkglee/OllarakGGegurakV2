// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditingHUD.h"

#include "CategoryUI.h"
#include "PropSlot.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/Utils/FastLogger.h"

void UMapEditingHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UMapEditingHUD::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	// 여기서 Delegate를 보내야 함.
	OnDragLeaveWidget.Broadcast();
	
	FFastLogger::LogScreen(FColor::Red, TEXT("Drag Leave"));
}

void UMapEditingHUD::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	OnDragEnterWidget.Broadcast();
	
	FFastLogger::LogScreen(FColor::Green, TEXT("Drag Enter"));
}

void UMapEditingHUD::InitWidget(UClickHandlerManager* ClickHandlerManager, UWidgetMapEditDragDropOperation* DragDropOperation)
{
	EditCategoryUI->InitWidget(ClickHandlerManager, DragDropOperation);

	AddDragDropFunctionsToDelegate(DragDropOperation);
	AddClickFunctionsToDelegate(ClickHandlerManager);
}

void UMapEditingHUD::AddClickFunctionsToDelegate(UClickHandlerManager* ClickHandlerManager)
{
	// TODO : 나중엔 모든 PropSlot에 대해서도 Delegate를 추가해야 함.
	// PropSlot->OnPropSlotClicked.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnPropSlotClicked);
	
	OnDragEnterWidget.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnWidgetDragEnter);
	OnDragLeaveWidget.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnWidgetDragLeave);
}

void UMapEditingHUD::AddDragDropFunctionsToDelegate(UWidgetMapEditDragDropOperation* DragDropOperation)
{
	OnDragEnterWidget.AddDynamic(DragDropOperation, &UWidgetMapEditDragDropOperation::OnDragEnter);
	OnDragLeaveWidget.AddDynamic(DragDropOperation, &UWidgetMapEditDragDropOperation::OnDragLeave);
}

void UMapEditingHUD::HideCategory()
{
	if (bIsCategoryHidden || !EditCategoryUI)
	{
		return;
	}
	ToggleBox->SetRenderTransformAngle(-180.f);
	PlayAnimation(CategoryHideAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.5f);
	bIsCategoryHidden = true;
}

void UMapEditingHUD::ShowCategory()
{
	if (!bIsCategoryHidden || !EditCategoryUI)
	{
		return;
	}
	ToggleBox->SetRenderTransformAngle(0.f);
	PlayAnimation(CategoryHideAnim, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.5f);
	bIsCategoryHidden = false;
}
