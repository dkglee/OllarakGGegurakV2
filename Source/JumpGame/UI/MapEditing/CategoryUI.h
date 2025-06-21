#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/MapEditor/CategorySystem/ECategoryType.h"
#include "CategoryUI.generated.h"

UCLASS()
class JUMPGAME_API UCategoryUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* DragDropOperation);

	UFUNCTION()
	void OnPropSlotClicked(FName PropID, class UClass* InPropClass);
private:
	void ClearSubCategoryButtons();
	void ClearMajorCategoryButtons();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UPropGridUI* GridUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UScrollBox* SubCategories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UScrollBox* MajorCategories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UEditableText* SearchText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UImage* PropOverviewImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* ImageSearchButton;

	UPROPERTY()
	class UMajorCategoryButtonUI* SelectedMajorCategory = nullptr;
	UPROPERTY()
	class USubCategoryButtonUI* SelectedSubCategory = nullptr;
	UPROPERTY()
	class UCategorySystem* CategorySystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UMajorCategoryButtonUI> MajorCategoryButtonUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class USubCategoryButtonUI> SubCategoryButtonUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UMapLoadingUI> MapLoadingUIClass;
	UPROPERTY()
	class UMapLoadingUI* ImageSearchLoadingUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class USaveResultUI> SaveResultUIClass;
	UPROPERTY()
	class USaveResultUI* SaveResultUI;
	
	

	UPROPERTY()
	TMap<EMajorCategoryType, class UMajorCategoryButtonUI*> MajorCategoryButtons;
	UPROPERTY()
	TMap<ESubCategoryType, class USubCategoryButtonUI*> SubCategoryButtons;
	
	UFUNCTION()
	void OnMajorCategoryButtonClicked(const EMajorCategoryType& MajorCategory, bool bAbsolute = false);
	UFUNCTION()
	void OnSubCategoryButtonClicked(const EMajorCategoryType& MajorCategory, const ESubCategoryType& SubCategory);
	UFUNCTION()
	void OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnSearchTextChanged(const FText& Text);
	UFUNCTION()
	void OnImageSearchButtonClicked();
	bool OpenFileDialog(FString& OutFilePath);
	bool SendImageRequest(const FString& ImagePath);
	void OnImageSearchResponse(bool bSuccess);
	void SetTextToDefault();
	void SetGridToDefault();
	void OnImageSearchButtonResponse(const FString& ImgPath, bool bSuccess);

	void ShowResultUI(bool bSuccess, const FString& ResultText);

	FString DefaultSearchText = TEXT("검색 할 내용을 입력하세요");

	FTimerHandle SearchTimerHandle;
	float SearchDelay = 0.5f;
};
