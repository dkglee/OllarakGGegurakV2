#include "VerticalPropSlotUI.h"

#include "CategoryUI.h"
#include "PropSlot.h"
#include "JumpGame/MapEditor/CategorySystem/PropWrap.h"

void UVerticalPropSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UVerticalPropSlotUI::InitWidget(UClickHandlerManager* ClickHandlerManager,
	class UWidgetMapEditDragDropOperation* CachedDragDropOperation, UCategoryUI* CategoryUI)
{
	PropTop->InitWidget(ClickHandlerManager, CachedDragDropOperation, CategoryUI);
	PropBelow->InitWidget(ClickHandlerManager, CachedDragDropOperation, CategoryUI);
}

void UVerticalPropSlotUI::SetPropSlots(UPropWrap* Top, UPropWrap* Below)
{
	PropTop->SetPropInfo(Top);
	PropBelow->SetPropInfo(Below);
}

void UVerticalPropSlotUI::ClearInfo()
{
	PropTop->ClearInfo();
	PropBelow->ClearInfo();
}
