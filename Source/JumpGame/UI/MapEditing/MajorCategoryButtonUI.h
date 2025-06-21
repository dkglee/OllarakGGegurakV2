#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/MapEditor/CategorySystem/ECategoryType.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "MajorCategoryButtonUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMajorButtonClicked, const EMajorCategoryType&, MajorCategory, bool, bAbsolute);

UCLASS()
class JUMPGAME_API UMajorCategoryButtonUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void SetMajorCategory(EMajorCategoryType MajorCategory);

	SETTER(EMajorCategoryType, MajorCategoryType)
	GETTER(EMajorCategoryType, MajorCategoryType)

	UPROPERTY(BlueprintAssignable)
	FOnMajorButtonClicked OnMajorButtonClicked;

	void ClearSelected();
	void SetSelected();
private:
	UFUNCTION()
	void OnMajorCategoryButtonClicked();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* MajorCategoryButton;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	// class UImage* MajorCategoryImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* MajorCategoryText;

	UPROPERTY()
	EMajorCategoryType MajorCategoryType = EMajorCategoryType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CategoryNameTable = nullptr;

	UPROPERTY()
	FButtonStyle NormalButtonStyle;
};
