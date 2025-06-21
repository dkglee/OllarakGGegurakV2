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
	PropTop->InitWidget(CachedDragDropOperation);
	PropTop->OnPropSlotClicked.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnPropSlotClicked);
	PropTop->OnPropSlotClicked.AddDynamic(CategoryUI, &UCategoryUI::OnPropSlotClicked);
	
	PropBelow->InitWidget(CachedDragDropOperation);
	PropBelow->OnPropSlotClicked.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnPropSlotClicked);
	PropBelow->OnPropSlotClicked.AddDynamic(CategoryUI, &UCategoryUI::OnPropSlotClicked);
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
