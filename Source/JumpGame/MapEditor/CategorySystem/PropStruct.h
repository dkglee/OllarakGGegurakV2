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
	class UTexture2D* PropIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	TSubclassOf<class APrimitiveProp> PropClass;

	FPropStruct(
		const FName&					InPropID = NAME_None,
		const FText&                   InPropName         = FText::GetEmpty(),
		const FText&                   InPropDescription  = FText::GetEmpty(),
		EMajorCategoryType             InPropCategory     = EMajorCategoryType::None,
		TArray<ESubCategoryType>       InPropSubCategories = {},
		TObjectPtr<const UTexture2D>   InPropIcon          = nullptr,
		TSubclassOf<class APrimitiveProp>           InPropClass         = nullptr)
		: PropID         (InPropID)
		, PropName        (InPropName)
		, PropDescription (InPropDescription)
		, PropCategory    (InPropCategory)
		, PropSubCategories(MoveTemp(InPropSubCategories))
		, PropIcon        (InPropIcon)
		, PropClass       (InPropClass)
	{}
};
