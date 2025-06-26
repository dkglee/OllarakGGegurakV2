// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSlot.h"

#include "CategoryUI.h"
#include "PropDragVisual.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/MapEditor/CategorySystem/PropWrap.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/DragDropOperation/WidgetMapEditDragDropOperation.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/Utils/CursorManager.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/KismetInputLibrary.h"

void UPropSlot::InitWidget(UClickHandlerManager* ClickHandlerManager, UWidgetMapEditDragDropOperation* CachedDragDropOperation, UCategoryUI
					* CategoryUI)
{
	DragDropOperation = CachedDragDropOperation;
	CachedClickHandlerManager = ClickHandlerManager;
	OnPropSlotClicked.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnPropSlotClicked);
	OnPropSlotClicked.AddDynamic(CategoryUI, &UCategoryUI::OnPropSlotClicked);
	
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
	PropCount = PropInfo->Data.PropCount;
	PropMaxCount = PropInfo->Data.PropMaxCount;
	SetPropCountText(PropID, PropInfo->Data.PropCount, PropInfo->Data.PropMaxCount);

	if (PropMaxCount > 0)
	{
		AMapEditorState* GS = Cast<AMapEditorState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->GetCategorySystem()->OnPropCountUpdated.AddDynamic(this, &UPropSlot::SetPropCountText);
		}
	}
	
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
	PropCount = -1;
	PropMaxCount = -1;
	PropCountText->SetText(FText::GetEmpty());
	AMapEditorState* GS = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->GetCategorySystem()->OnPropCountUpdated.RemoveDynamic(this, &UPropSlot::SetPropCountText);
	}
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
	// 갯수가 없으면 Drag를 시작하지 않음
	if (PropCount <= 0 && PropMaxCount > 0)
	{
		return ;
	}
	// PropCount를 깍음
	PropCount -= 1;
	
	AMapEditorState* GS = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->GetCategorySystem()->UpdatePropCountByID(PropID, PropCount);
	}
	
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	OutOperation = DragDropOperation;
	CachedClickHandlerManager->OnPropDragCancelledDelegate.BindDynamic(this, &UPropSlot::OnPropDragCancelled);
}

void UPropSlot::SetPropCountText(FName ID, int32 Count, int32 MaxCount)
{
	// SetText가 필요가 없음
	if (MaxCount <= 0)
	{
		PropCountText->SetText(FText::GetEmpty());
		return ;
	}

	if (ID != PropID)
	{
		// ID가 다르면 아무것도 하지 않음
		return;
	}
	PropCount = Count;
	FString CountText = FString::Printf(TEXT("%d/%d"), Count, MaxCount);
	PropCountText->SetText(FText::FromString(CountText));
}

void UPropSlot::OnPropDragCancelled()
{
	PropCount += 1; // Drag가 취소되었으므로 PropCount를 증가시킴

	AMapEditorState* GS = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->GetCategorySystem()->UpdatePropCountByID(PropID, PropCount);
	}
}
