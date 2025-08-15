#pragma once

#include "CoreMinimal.h"
#include "ECategoryType.h"
#include "PropStruct.generated.h"

// DataTable Struct
USTRUCT(BlueprintType)
struct FPropStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FName PropID; // RowName 이랑 같은 값으로 설정해야 함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FText PropName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FText PropDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	EMajorCategoryType PropCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	TArray<ESubCategoryType> PropSubCategories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	bool bIsHidden; // 숨김 여부, true면 카테고리에서 안 보임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	int32 PropMaxCount; // -1이면 무제한, 0이면 사용 불가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	int32 PropCount; // 현재 소지 개수, -1이면 무제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	class UTexture2D* PropIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	TSubclassOf<class APrimitiveProp> PropClass;

	FPropStruct(
		const FName&					InPropID = NAME_None,
		const FText&                   InPropName         = FText::GetEmpty(),
		const FText&                   InPropDescription  = FText::GetEmpty(),
		EMajorCategoryType             InPropCategory     = EMajorCategoryType::None,
		TArray<ESubCategoryType>       InPropSubCategories = {},
		bool                           bInIsHidden        = false,
		int32						   InPropMaxCount     = -1,
		int32						   InPropCount        = -1,
		TObjectPtr<const UTexture2D>   InPropIcon          = nullptr,
		TSubclassOf<class APrimitiveProp>           InPropClass         = nullptr)
		: PropID         (InPropID)
		, PropName        (InPropName)
		, PropDescription (InPropDescription)
		, PropCategory    (InPropCategory)
		, PropSubCategories(MoveTemp(InPropSubCategories))
		, bIsHidden       (bInIsHidden)
		, PropMaxCount    (InPropMaxCount)
		, PropCount       (InPropCount)
		, PropIcon        (InPropIcon)
		, PropClass       (InPropClass)
	{}
};
