#pragma once

#include "CoreMinimal.h"
#include "PropStruct.h"
#include "UObject/Object.h"
#include "PropWrap.generated.h"

UCLASS()
class JUMPGAME_API UPropWrap : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPropStruct Data;
};
