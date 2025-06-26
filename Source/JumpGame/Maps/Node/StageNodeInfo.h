#pragma once

#include "CoreMinimal.h"
#include "StageNodeInfo.generated.h"

USTRUCT(BlueprintType)
struct FStageNodeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NodeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasField; // 필드 존재 여부
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FieldName; // 필드 있다면, 이름

	FStageNodeInfo()
		: NodeID(0)
		, WorldPosition(FVector::ZeroVector)
		, bHasField(false)
		, FieldName(NAME_None)
	{}
};