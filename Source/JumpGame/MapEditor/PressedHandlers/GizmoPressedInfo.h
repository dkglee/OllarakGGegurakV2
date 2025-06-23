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
	TMap<class APrimitiveProp*, FVector> OriginalActorLocations;
	
	FGizmoPressedInfo(const FVector& InInitialMouseRayOrigin = FVector::ZeroVector, const FVector& InInitialMouseRayDirection = FVector::ZeroVector, const TMap<class APrimitiveProp*, FVector>& InOriginalActorLocations = TMap<class APrimitiveProp*, FVector>())
		: InitialMouseRayOrigin(InInitialMouseRayOrigin)
		, InitialMouseRayDirection(InInitialMouseRayDirection)
		, OriginalActorLocations(InOriginalActorLocations)
	{
	}
};