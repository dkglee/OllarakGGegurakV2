#pragma once

#include "CoreMinimal.h"
#include "CategoryName.generated.h"

USTRUCT(BlueprintType)
struct FCategoryName : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FString CategoryName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	bool bHidden = false;

	FCategoryName(
		const FString& InCategoryName = TEXT(""),
		const FText& InDisplayName = FText::GetEmpty(),
		bool InHidden = false)
		: CategoryName(InCategoryName)
		, DisplayName(InDisplayName)
		, bHidden(InHidden)
	{
	}
};
