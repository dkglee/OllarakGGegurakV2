#pragma once

#include "CoreMinimal.h"
#include "FieldTableRow.generated.h"

// DataTable Struct
USTRUCT(BlueprintType)
struct FFieldTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FName FieldID; // 필드 식별자
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FName StageID; // 소속 스테이지 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FText FieldName; // UI용 필드 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FString FieldFilePath; // 필드 파일 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	class UTexture2D* Thumbnail; // 필드 썸네일 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FText Description; // 필드 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FName ClearCondition; // 필드 클리어 조건 (예: 별 개수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FName UnlockCondition; // 필드 잠금 해제 조건 (예: 특정 필드 완료 후)

	FFieldTableRow(
		const FName& InFieldID = NAME_None,
		const FName& InStageID = NAME_None,
		const FText& InFieldName = FText::GetEmpty(),
		const FString& InFieldFilePath = TEXT(""),
		TObjectPtr<const UTexture2D> InThumbnail = nullptr,
		const FText& InDescription = FText::GetEmpty(),
		const FName& InClearCondition = NAME_None,
		const FName& InUnlockCondition = NAME_None)
		: FieldID(InFieldID)
		, StageID(InStageID)
		, FieldName(InFieldName)
		, FieldFilePath(InFieldFilePath)
		, Thumbnail(InThumbnail)
		, Description(InDescription)
		, ClearCondition(InClearCondition)
		, UnlockCondition(InUnlockCondition)
	{}
};