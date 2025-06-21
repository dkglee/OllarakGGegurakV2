#pragma once

#include "CoreMinimal.h"
#include "ECategoryType.h"
#include "MajorTableInfo.generated.h"

USTRUCT(BlueprintType)
struct FMajorTableInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	EMajorCategoryType MajorCategoryType = EMajorCategoryType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	TArray<ESubCategoryType> SubCategoryTypes = {};

	FMajorTableInfo(
		EMajorCategoryType InMajorCategoryType = EMajorCategoryType::None,
		TArray<ESubCategoryType, TFixedAllocator<8>> InSubCategoryTypes = {})
		: MajorCategoryType(InMajorCategoryType)
		, SubCategoryTypes(MoveTemp(InSubCategoryTypes))
	{}
};
