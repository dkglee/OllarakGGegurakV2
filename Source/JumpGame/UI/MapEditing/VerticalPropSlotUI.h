#pragma once

#include "CoreMinimal.h"
#include "CategoryUI.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/MapEditor/CategorySystem/PropStruct.h"
#include "JumpGame/MapEditor/CategorySystem/PropWrap.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "VerticalPropSlotUI.generated.h"

UCLASS()
class JUMPGAME_API UVerticalPropSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* CachedDragDropOperation, class UCategoryUI
	                * CategoryUI);
	
	void SetPropSlots(class UPropWrap* Top, class UPropWrap* Below = nullptr);
	void ClearInfo();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UPropSlot* PropTop;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UPropSlot* PropBelow;
};
