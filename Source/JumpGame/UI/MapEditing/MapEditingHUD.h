#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapEditingHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDragEnterWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDragLeaveWidget);

UCLASS()
class JUMPGAME_API UMapEditingHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* DragDropOperation);
	void AddClickFunctionsToDelegate(class UClickHandlerManager* ClickHandlerManager);
	void AddDragDropFunctionsToDelegate(class UWidgetMapEditDragDropOperation* DragDropOperation);

	UFUNCTION(BlueprintCallable)
	void HideCategory();
	UFUNCTION(BlueprintCallable)
	void ShowCategory();

	UPROPERTY(BlueprintAssignable)
	FOnDragEnterWidget OnDragEnterWidget;
	UPROPERTY(BlueprintAssignable)
	FOnDragLeaveWidget OnDragLeaveWidget;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UCategoryUI* EditCategoryUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient, meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UWidgetAnimation* CategoryHideAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class USizeBox* ToggleBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	bool bIsCategoryHidden = false;
};
