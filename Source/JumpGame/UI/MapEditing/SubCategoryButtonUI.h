#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/MapEditor/CategorySystem/ECategoryType.h"
#include "SubCategoryButtonUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubCategoryButtonClicked, const EMajorCategoryType&, InMajorCategory, const ESubCategoryType&, InSubCategory);

UCLASS()
class JUMPGAME_API USubCategoryButtonUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void SetSubCategoryInfo(EMajorCategoryType InMajorCategory, ESubCategoryType InSubCategory);
	UFUNCTION()
	void SubCategoryButtonClicked();
	
	void ClearSelected();
	void SetSelected();

	UPROPERTY(BlueprintAssignable)
	FOnSubCategoryButtonClicked OnSubCategoryButtonClicked;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* SubCategoryButton;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	// class UImage* SubCategoryImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* SubCategoryText;

	UPROPERTY()
	EMajorCategoryType MajorCategory;
	UPROPERTY()
	ESubCategoryType SubCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CategoryNameTable = nullptr;

	UPROPERTY()
	FButtonStyle NormalButtonStyle;
};
