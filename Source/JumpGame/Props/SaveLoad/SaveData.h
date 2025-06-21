#pragma once

#include "CoreMinimal.h"
#include "SaveData.generated.h"

USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Size;

	FSaveData(FName InId = NAME_None, FVector InPosition = FVector::ZeroVector, FRotator InRotation = FRotator::ZeroRotator, FVector InSize = FVector::ZeroVector)
		: Id(InId)
		, Position(InPosition)
		, Rotation(InRotation)
		, Size(InSize)
	{
	}
};

USTRUCT(BlueprintType)
struct FSaveDataArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ImageBase64;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSaveData> SaveDataArray;

	FSaveDataArray(TArray<FSaveData> InSaveDataArray = {})
		: SaveDataArray(MoveTemp(InSaveDataArray))
	{
	}
};