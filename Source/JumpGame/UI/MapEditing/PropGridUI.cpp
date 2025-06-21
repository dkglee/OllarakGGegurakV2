#include "PropGridUI.h"

#include "VerticalPropSlotUI.h"
#include "Components/ScrollBox.h"

void UPropGridUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PropSlots.Empty();

	ClearPropGrid();
	
	for (int32 i = 0; i < PropStack; i++)
	{
		UVerticalPropSlotUI* PropSlot = CreateWidget<UVerticalPropSlotUI>(GetWorld(), VerticalPropSlotUIClass);
		PropSlots.Add(PropSlot);
	}
}

void UPropGridUI::InitWidget(class UClickHandlerManager* ClickHandlerManager,
	class UWidgetMapEditDragDropOperation* DragDropOperation, UCategoryUI* CategoryUI)
{
	CachedClickHandlerManager = ClickHandlerManager;
	CachedDragDropOperation = DragDropOperation;

	for (auto& PropSlot : PropSlots)
	{
		PropSlot->InitWidget(CachedClickHandlerManager, CachedDragDropOperation, CategoryUI);
	}
}

void UPropGridUI::ClearPropGrid()
{
	GridScroll->ClearChildren();
	
	for (auto& PropSlot : PropSlots)
	{
		PropSlot->ClearInfo();
	}
}

void UPropGridUI::UpdatePropGrid(const EMajorCategoryType& MajorCategory, UCategorySystem* CategorySystem)
{
	ClearPropGrid();

	const TArray<UPropWrap*>& PropList = CategorySystem->GetPropsByMajor(MajorCategory);
	
	int32 SlotIndex = 0;
	for (int32 i = 0; i < PropList.Num(); i += 2)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList[i], i + 1 < PropList.Num() ? PropList[i + 1] : nullptr);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
}

void UPropGridUI::UpdatePropGridBySub(const EMajorCategoryType& MajorCategory, ESubCategoryType SubCategory,
	UCategorySystem* CategorySystem)
{
	ClearPropGrid();

	const TArray<UPropWrap*>& PropList = CategorySystem->GetPropsBySub(MajorCategory, SubCategory);

	int32 SlotIndex = 0;
	for (int32 i = 0; i < PropList.Num(); i += 2)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList[i], i + 1 < PropList.Num() ? PropList[i + 1] : nullptr);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
}

void UPropGridUI::UpdatePropGridBySearch(const FString& String, UCategorySystem* CategorySystem)
{
	ClearPropGrid();

	FName PropName = FName(*String);
	const TArray<UPropWrap*>& PropList = CategorySystem->GetPropsByName(PropName);

	int32 SlotIndex = 0;
	for (int32 i = 0; i < PropList.Num(); i += 2)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList[i], i + 1 < PropList.Num() ? PropList[i + 1] : nullptr);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
}

void UPropGridUI::UpdatePropGridByImageSearch(const TArray<ESubCategoryType>& SubCategories, UCategorySystem* CategorySystem)
{
	ClearPropGrid();

	const TArray<UPropWrap*>& PropList = CategorySystem->GetPropsBySubs(SubCategories);
	
	int32 SlotIndex = 0;
	for (int32 i = 0; i < PropList.Num(); i += 2)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList[i], i + 1 < PropList.Num() ? PropList[i + 1] : nullptr);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
}
