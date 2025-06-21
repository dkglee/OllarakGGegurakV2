#pragma once

#include "CoreMinimal.h"
#include "IOHandlerInitInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct JUMPGAME_API FIOHandlerInitInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IO")
	FString ServerUrl = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IO")
	FString ServerProtocol = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IO")
	FString ReadPipeName = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IO")
	FString SendPipeName = TEXT("");

	FIOHandlerInitInfo(const FString& InServerUrl = TEXT(""), const FString& InServerProtocol = TEXT(""), const FString& InReadPipeName = TEXT(""), const FString& InSendPipeName = TEXT(""))
		: ServerUrl(InServerUrl), ServerProtocol(InServerProtocol), ReadPipeName(InReadPipeName), SendPipeName(InSendPipeName)
	{
	}
};
