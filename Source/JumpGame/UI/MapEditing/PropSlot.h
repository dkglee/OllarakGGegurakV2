#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/MapEditor/CategorySystem/PropStruct.h"
#include "JumpGame/MapEditor/CategorySystem/PropWrap.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "PropSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPropSlotClicked, FName, PropID, class UClass*, InPropClass);

UCLASS()
class JUMPGAME_API UPropSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	// SETTER(class UWidgetMapEditDragDropOperation*, DragDropOperation);
	void InitWidget(class UWidgetMapEditDragDropOperation* InDragDropOperation);
	void SetPropID(FName InPropID);
	void SetPropInfo(class UPropWrap* PropInfo);
	void ClearInfo();

	UPROPERTY(BlueprintAssignable, Category = "Prop")
	FOnPropSlotClicked OnPropSlotClicked;
	
protected:
	virtual void NativeOnInitialized() override;

	// 해당 위젯이 클릭되었을 때 호출되는 함수
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// 해당 위젯에서 Drag가 감지 되었을 때 호출되는 함수
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FName PropID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* PropText;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	TSubclassOf<class UUserWidget> PropWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	class UPropDragVisual* PropDragVisual = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	class UWidgetMapEditDragDropOperation* DragDropOperation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UImage* PropImage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UClass* PropClass;
};
