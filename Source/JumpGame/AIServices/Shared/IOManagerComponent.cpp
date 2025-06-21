#include "IOManagerComponent.h"

#include "JsonObjectConverter.h"
#include "IOHandlers/HttpsMultiPartsHandler.h"
#include "IOHandlers/IPCHandler.h"
#include "IOHandlers/SocketHandler.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "JumpGame/Utils/FastLogger.h"

UIOManagerComponent::UIOManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UIOManagerComponent::SendGameMessage(const FMessageUnion& Message)
{
	EMessageType MessageType = Message.Header.Type;
	if (!IOHandlers.Contains(MessageType))
	{
		return false;
	}

	FFastLogger::LogConsole(TEXT("UIOManagerComponent::SendGameMessage"));
	IOHandlers[MessageType]->SendGameMessage(Message);
	return true;
}

void UIOManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InitializeConfigFile())
	{
		return ;
	}
	
	// TODO: 방어코드 재설정
	if (!GetWorld()->GetMapName().Contains(TEXT("InGameLevel")))
	{
		return ;
	}
	
	// IOHandler 초기화
	TSharedPtr<IIOHandlerInterface> IPCReadHandler = MakeShared<FIPCHandler>();
	IPCReadHandler->SetReaderMode();
	TSharedPtr<IIOHandlerInterface> IPCSendHandler = MakeShared<FIPCHandler>();
	IPCSendHandler->SetWriterMode();
	TSharedPtr<IIOHandlerInterface> SocketHandler = MakeShared<FSocketHandler>();
	
	RegisterIOHandler(EMessageType::Ping, SocketHandler);
	RegisterIOHandler(EMessageType::Pong, SocketHandler);
	RegisterIOHandler(EMessageType::QuizNotify, SocketHandler);
	RegisterIOHandler(EMessageType::WaveRequest, SocketHandler);
	RegisterIOHandler(EMessageType::WaveResponse, SocketHandler);
	
	RegisterIOHandler(EMessageType::EyeTrackingNotifyMessage, IPCSendHandler);
	RegisterIOHandler(EMessageType::EyeTrackingResponseMessage, IPCReadHandler);
	RegisterIOHandler(EMessageType::EyeTrackingRequestMessage, IPCReadHandler);

	// queue 초기화
	for (auto& Handler : IOHandlers)
	{
		MessageQueue[Handler.Key] = std::queue<FMessageUnion>();
	}

	if (!IPCReadHandler->Init(IOHandlerInitInfo, &MessageQueue, nullptr))
	{
		TSharedPtr<FIPCHandler> SharedIPC = StaticCastSharedPtr<FIPCHandler>(IPCReadHandler);
		RetryReadConnectToPipe(SharedIPC);
	}

	if (!IPCSendHandler->Init(IOHandlerInitInfo, &MessageQueue, nullptr))
	{
		TSharedPtr<FIPCHandler> SharedIPC = StaticCastSharedPtr<FIPCHandler>(IPCSendHandler);
		RetrySendConnectToPipe(SharedIPC);
	}

	SocketHandler->Init(IOHandlerInitInfo, &MessageQueue, nullptr);
}

void UIOManagerComponent::RegisterIOHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler)
{
	IOHandlers.Add(MessageType, Handler);
}

bool UIOManagerComponent::InitializeConfigFile()
{
	FString ConfigFilePath = FPaths::ProjectConfigDir() + TEXT("IOHandlerConfig.json");
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
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(ConfigContent, &IOHandlerInitInfo))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Failed to parse config file: %s"), *ConfigFilePath);
		return false;
	}
	
	return true;
}

void UIOManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto& Handler : IOHandlers)
	{
		Handler.Value->ReceiveMessage();
	}
}

bool UIOManagerComponent::PopMessage(const EMessageType& MessageType, FMessageUnion& OutMessage)
{
	if (MessageQueue.find(MessageType) != MessageQueue.end())
	{
		if (MessageQueue[MessageType].empty())
		{
			return false;
		}
		FMessageUnion Message = MessageQueue[MessageType].front();
		MessageQueue[MessageType].pop();
		OutMessage = Message;
		return true;
	}
	return false;
}

void UIOManagerComponent::RetryReadConnectToPipe(TSharedPtr<FIPCHandler> IPCHandlerToRetry)
{
	TWeakObjectPtr<UIOManagerComponent> WeakThis = this;

	GetWorld()->GetTimerManager().SetTimer(RetryReadTimer, FTimerDelegate::CreateLambda([WeakThis, IPCHandlerToRetry]()
	{
		if (!WeakThis.IsValid())
			return;

		UIOManagerComponent* StrongThis = WeakThis.Get();
		if (IPCHandlerToRetry->Init(StrongThis->IOHandlerInitInfo, &StrongThis->MessageQueue, nullptr))
		{
			FFastLogger::LogConsole(TEXT("PIPE!!!!!!!!!!! Succedded!! IPCHandler : %s"), *IPCHandlerToRetry->GetPipeName());
		}
		else
		{
			//FFastLogger::LogConsole(TEXT("Reconnecting to pipe... : %s"), *IPCHandlerToRetry->GetPipeName());
			StrongThis->RetryReadConnectToPipe(IPCHandlerToRetry);
		}
	}), RetryInterval, false);
}

void UIOManagerComponent::RetrySendConnectToPipe(TSharedPtr<FIPCHandler> IPCHandlerToRetry)
{
	TWeakObjectPtr<UIOManagerComponent> WeakThis = this;

	GetWorld()->GetTimerManager().SetTimer(RetrySendTimer, FTimerDelegate::CreateLambda([WeakThis, IPCHandlerToRetry]()
	{
		if (!WeakThis.IsValid())
			return;

		UIOManagerComponent* StrongThis = WeakThis.Get();
		if (IPCHandlerToRetry->Init(StrongThis->IOHandlerInitInfo, &StrongThis->MessageQueue, nullptr))
		{
			FFastLogger::LogConsole(TEXT("PIPE!!!!!!!!!!! Succedded!! IPCHandler : %s"), *IPCHandlerToRetry->GetPipeName());
		}
		else
		{
			//FFastLogger::LogConsole(TEXT("Reconnecting to pipe... : %s"), *IPCHandlerToRetry->GetPipeName());
			StrongThis->RetrySendConnectToPipe(IPCHandlerToRetry);
		}
	}), RetryInterval, false);
}
