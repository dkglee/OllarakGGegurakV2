#pragma once

#include "CoreMinimal.h"
#include "HTTPHandlerInitInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct JUMPGAME_API FHTTPHandlerInitInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP")
	FString HttpsRequestImageURL = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP")
	FString HttpsRequestImagePath = TEXT("");


	FHTTPHandlerInitInfo(const FString& InHttpsRequestImageURL = TEXT(""), const FString& InHttpsRequestImagePath = TEXT(""))
		: HttpsRequestImageURL(InHttpsRequestImageURL), HttpsRequestImagePath(InHttpsRequestImagePath)
	{
	}
};
