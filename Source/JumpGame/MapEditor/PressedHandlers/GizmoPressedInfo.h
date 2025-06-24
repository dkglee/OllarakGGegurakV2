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

	UPROPERTY(Blueprintable)
	uint8 Flags;
	enum : uint8
	{
		CopyMode = 1 << 0,  // 0x01
		AlreadyCopied = 1 << 1, // 0x02
	};
	
	FGizmoPressedInfo(const FVector& InInitialMouseRayOrigin = FVector::ZeroVector, const FVector& InInitialMouseRayDirection = FVector::ZeroVector, const TMap<class APrimitiveProp*, FVector>& InOriginalActorLocations = TMap<class APrimitiveProp*, FVector>(), uint8 InFlags = 0)
		: InitialMouseRayOrigin(InInitialMouseRayOrigin)
		, InitialMouseRayDirection(InInitialMouseRayDirection)
		, OriginalActorLocations(InOriginalActorLocations)
		, Flags(InFlags)
	{
	}

	FORCEINLINE bool Has(uint8 Flag) const { return (Flags & Flag) != 0; }
};