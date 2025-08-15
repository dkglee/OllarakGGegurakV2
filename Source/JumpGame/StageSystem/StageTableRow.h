#pragma once

#include "CoreMinimal.h"
#include "StageTableRow.generated.h"

USTRUCT(BlueprintType)
struct FStageTableRow : public FTableRowBase
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FName StageID; // 스테이지 식별자
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FText StageName; // UI용 스테이지 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FText StageDescription; // 스테이지 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FName StageWorld; // 스테이지 월드 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	class UTexture2D* StageThumbnail; // 스테이지 썸네일 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	TArray<FName> FieldLists; // 소속 필드 ID 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FName UnlockCondition; // 스테이지 잠금 해제 조건 (예: 특정 필드 완료 후)

	FStageTableRow(
		const FName& InStageID = NAME_None,
		const FText& InStageName = FText::GetEmpty(),
		const FText& InStageDescription = FText::GetEmpty(),
		FName InStageWorld = NAME_None,
		TObjectPtr<const UTexture2D> InStageThumbnail = nullptr,
		TArray<FName> InFieldLists = {},
		const FName& InUnlockCondition = NAME_None)
		: StageID(InStageID)
		, StageName(InStageName)
		, StageDescription(InStageDescription)
		, StageWorld(InStageWorld)
		, StageThumbnail(InStageThumbnail)
		, FieldLists(MoveTemp(InFieldLists))
		, UnlockCondition(InUnlockCondition)
	{}
};
