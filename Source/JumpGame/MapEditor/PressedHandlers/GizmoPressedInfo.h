#pragma once

#include "CoreMinimal.h"
#include "GizmoPressedInfo.generated.h"

USTRUCT(BlueprintType)
struct JUMPGAME_API FGizmoPressedInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(Blueprintable)
	FVector InitialMouseRayOrigin = FVector::ZeroVector;
	
	UPROPERTY(Blueprintable)
	FVector InitialMouseRayDirection = FVector::ZeroVector;
	
	UPROPERTY(Blueprintable)
	FVector OriginalActorLocation = FVector::ZeroVector;
	
	FGizmoPressedInfo(const FVector& InInitialMouseRayOrigin = FVector::ZeroVector, const FVector& InInitialMouseRayDirection = FVector::ZeroVector, const FVector& InOriginalActorLocation = FVector::ZeroVector)
		: InitialMouseRayOrigin(InInitialMouseRayOrigin)
		, InitialMouseRayDirection(InInitialMouseRayDirection)
		, OriginalActorLocation(InOriginalActorLocation)
	{
	}
};