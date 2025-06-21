#pragma once

#include "CoreMinimal.h"
#include "ECategoryType.h"
#include "ImageResponseJson.generated.h"

USTRUCT(BlueprintType)
struct JUMPGAME_API FImageResponseJson
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ESubCategoryType> SubCategoryList;

	FImageResponseJson(
		const TArray<ESubCategoryType>& InSubCategoryList = {})
		: SubCategoryList(InSubCategoryList)
	{
	}
};
