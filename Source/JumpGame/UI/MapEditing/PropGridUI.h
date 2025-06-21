#pragma once

#include "CoreMinimal.h"
#include "CategoryUI.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/MapEditor/CategorySystem/ECategoryType.h"
#include "PropGridUI.generated.h"

UCLASS()
class JUMPGAME_API UPropGridUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* DragDropOperation, class UCategoryUI*
	                CategoryUI);
	UFUNCTION()
	void UpdatePropGrid(const EMajorCategoryType& MajorCategory, class UCategorySystem* CategorySystem);
	UFUNCTION()
	void UpdatePropGridBySub(const EMajorCategoryType& MajorCategory, ESubCategoryType SubCategory, class UCategorySystem* CategorySystem);
	UFUNCTION()
	void UpdatePropGridBySearch(const FString& String, class UCategorySystem* CategorySystem);
	UFUNCTION()
	void UpdatePropGridByImageSearch(const TArray<ESubCategoryType>& SubCategories, class UCategorySystem* CategorySystem);
	UFUNCTION()
	void ClearPropGrid();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UScrollBox* GridScroll;

	UPROPERTY()
	class UClickHandlerManager* CachedClickHandlerManager = nullptr;
	UPROPERTY()
	class UWidgetMapEditDragDropOperation* CachedDragDropOperation = nullptr;
	
	UPROPERTY()
	TArray<class UVerticalPropSlotUI*> PropSlots;
	UPROPERTY()
	int32 PropStack = 70;
	// 왜 VerticalPropSlotUI로 안되는거임?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UUserWidget> VerticalPropSlotUIClass;
};
