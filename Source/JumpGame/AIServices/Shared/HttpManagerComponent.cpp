#include "HttpManagerComponent.h"

#include "JsonObjectConverter.h"
#include "IOHandlers/HttpsMultiPartsHandler.h"
#include "JumpGame/Utils/FastLogger.h"

UHttpManagerComponent::UHttpManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHttpManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InitializeConfigFile())
	{
		return ;
	}

	// TODO : HttpManagerComponent를 두자
	TSharedPtr<IIOHandlerInterface> HttpMultipartHandler = MakeShared<FHttpsMultiPartsHandler>();
	
	RegisterHttpHandler(EMessageType::HttpMultipartRequest, HttpMultipartHandler);

	for (auto& Handler : HttpHandlers)
	{
		HttpMessageQueue[Handler.Key] = std::queue<FHttpMessageWrapper>();
	}
	HttpMessageQueue[EMessageType::HttpMultipartResponse] = std::queue<FHttpMessageWrapper>();

// 	// 목업 데이터 하나 넣어보자!
// 	FHttpMessageWrapper Message;
// 	Message.Header.Type = EMessageType::HttpMultipartResponse;
// 	Message.HttpMessage = FHttpMultipartResponse();
// 	FHttpMultipartResponse& HttpMultipartResponseRef = std::get<FHttpMultipartResponse>(Message.HttpMessage);
// 	HttpMultipartResponseRef.ResponseCode = 200;
// 	HttpMultipartResponseRef.ResponseText = TEXT(R"({
// "SubCategoryList": [102]
// })");

	// HttpMessageQueue[EMessageType::HttpMultipartResponse].push(Message);
	
	HttpMultipartHandler->Init(FIOHandlerInitInfo(), nullptr, &HttpMessageQueue);
}


bool UHttpManagerComponent::SendHttpMessage(const FHttpMessageWrapper& HttpMessage)
{
	EMessageType MessageType = HttpMessage.Header.Type;
	if (!HttpHandlers.Contains(MessageType))
	{
		return false;
	}
	
	FFastLogger::LogConsole(TEXT("UIOManagerComponent::SendHttpMessage"));
	HttpHandlers[MessageType]->SendGameMessage(HttpMessage);
	return true;
}

void UHttpManagerComponent::RegisterHttpHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler)
{
	FFastLogger::LogConsole(TEXT("UIOManagerComponent::RegisterHttpHandler : %d"), MessageType);
	HttpHandlers.Add(MessageType, Handler);
}

bool UHttpManagerComponent::InitializeConfigFile()
{
	FString ConfigFilePath = FPaths::ProjectConfigDir() + TEXT("HttpHandlerConfig.json");
	if (!FPaths::FileExists(ConfigFilePath))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Config file not found: %s"), *ConfigFilePath);
		return false;
	}

	FString ConfigContent;
	FFileHelper::LoadFileToString(ConfigContent, *ConfigFilePath);
	if (ConfigContent.IsEmpty())
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Config file is empty: %s"), *ConfigFilePath);
		return false;
	}
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(ConfigContent, &HttpHandlerInitInfo))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Failed to parse config file: %s"), *ConfigFilePath);
		return false;
	}

	return true;
}

bool UHttpManagerComponent::PopHttpMessage(const EMessageType& MessageType, FHttpMessageWrapper& OutMessage)
{
	if (HttpMessageQueue.find(MessageType) != HttpMessageQueue.end())
	{
		if (HttpMessageQueue[MessageType].empty())
		{
			return false;
		}
		FHttpMessageWrapper Message = HttpMessageQueue[MessageType].front();
		HttpMessageQueue[MessageType].pop();
		OutMessage = Message;
		return true;
	}
	return false;
}
