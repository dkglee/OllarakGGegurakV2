// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSlot.h"

#include "PropDragVisual.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/MapEditor/CategorySystem/PropWrap.h"
#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/Utils/CursorManager.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/KismetInputLibrary.h"

void UPropSlot::InitWidget(UWidgetMapEditDragDropOperation* InDragDropOperation)
{
	DragDropOperation = InDragDropOperation;
}

void UPropSlot::SetPropID(FName InPropID)
{
	PropID = InPropID;
}

void UPropSlot::SetPropInfo(UPropWrap* PropInfo)
{
	if (!PropInfo)
	{
		return;
	}
	PropID = PropInfo->Data.PropID;
	PropImage->SetBrushFromTexture(PropInfo->Data.PropIcon);
	PropClass = PropInfo->Data.PropClass;
	PropText->SetText(PropInfo->Data.PropName);
	this->SetVisibility(ESlateVisibility::Visible);
	// DragVisual을 세팅해줘야 함.
	PropDragVisual->PropImage->SetBrushFromTexture(PropInfo->Data.PropIcon);
}

void UPropSlot::ClearInfo()
{
	PropID = NAME_None;
	PropImage->SetBrushFromTexture(nullptr);
	PropClass = nullptr;
	PropText->SetText(FText::GetEmpty());
	this->SetVisibility(ESlateVisibility::Collapsed);
	PropDragVisual->PropImage->SetBrushFromTexture(nullptr);
}

void UPropSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	PropDragVisual = CreateWidget<UPropDragVisual>(GetWorld(), PropWidgetClass);
}

// 마우스가 눌렸음을 감지함.
FReply UPropSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	if (!UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{
		return FReply::Unhandled();
	}
	if (PropID == NAME_None)
	{
		return FReply::Unhandled();
	}
	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

	FFastLogger::LogScreen(FColor::Orange, TEXT("PropSlot::NativeOnPreviewMouseButtonDown"));

	// DragDropOperation을 새롭게 세팅함
	// @fixme 왜 DragDropOperation이 없지?
	DragDropOperation->DefaultDragVisual = PropDragVisual;
	
	// 여기서 어떤 Prop이 선택되었는 지를 BroadCast 해준다.
	OnPropSlotClicked.Broadcast(PropID, PropClass);

	UCursorManager::SetCursor(this, ECursorName::LeafPoint);
	
	return Reply.NativeReply;
}

FReply UPropSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UCursorManager::SetCursor(this, ECursorName::LeafCursor);

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UPropSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                     UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	OutOperation = DragDropOperation;
	
	FFastLogger::LogScreen(FColor::Orange, TEXT("PropSlot::NativeOnDragDetected"));
}
