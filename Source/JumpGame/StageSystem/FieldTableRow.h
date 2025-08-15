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
	class UTexture2D* FieldMemoryImage; // 필드 메모리 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FText Description; // 필드 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FName ClearCondition; // 필드 클리어 조건 (예: 별 개수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FName UnlockCondition; // 필드 잠금 해제 조건 (예: 특정 필드 완료 후)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	int32 FieldStarCount; // 필드에서 획득한 별 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	float FieldClearTime; // 필드 클리어 기록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	FName FieldLevel; // 필드

	FFieldTableRow(
		const FName& InFieldID = NAME_None,
		const FName& InStageID = NAME_None,
		const FText& InFieldName = FText::GetEmpty(),
		const FString& InFieldFilePath = TEXT(""),
		TObjectPtr<const UTexture2D> InFieldMemoryImage = nullptr,
		const FText& InDescription = FText::GetEmpty(),
		const FName& InClearCondition = NAME_None,
		const FName& InUnlockCondition = NAME_None)
		: FieldID(InFieldID)
		, StageID(InStageID)
		, FieldName(InFieldName)
		, FieldFilePath(InFieldFilePath)
		, FieldMemoryImage(InFieldMemoryImage)
		, Description(InDescription)
		, ClearCondition(InClearCondition)
		, UnlockCondition(InUnlockCondition)
		, FieldStarCount(0)
		, FieldClearTime(0.f)
		, FieldLevel(NAME_None)
	{}
};