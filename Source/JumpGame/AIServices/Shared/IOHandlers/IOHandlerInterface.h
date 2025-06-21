#pragma once

#include <map>
#include <queue>

#include "CoreMinimal.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "IOHandlerInitInfo.h"
#include "JumpGame/AIServices/Shared/HttpMessage.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMessageReceived, bool /*bSuccess*/);

// Socket, IPC에 따라서 분기를 나누는 역할
class JUMPGAME_API IIOHandlerInterface
{
public:
	IIOHandlerInterface() {};

	typedef std::map<EMessageType, std::queue<FMessageUnion>>* MessageQueuePtr;
	typedef std::map<EMessageType, std::queue<FHttpMessageWrapper>>* HttpMessageQueuePtr;
	
	virtual bool Init(const FIOHandlerInitInfo& InitInfo, MessageQueuePtr InMessageQueuePtr, HttpMessageQueuePtr InHttpMessageQueuePtr) = 0;
	virtual bool SendGameMessage(const FMessageUnion& Message) = 0;
	virtual bool SendGameMessage(const FHttpMessageWrapper& HttpMessage) = 0;
	virtual bool ReceiveMessage() = 0;
	virtual void SetReaderMode() {};
	virtual void SetWriterMode() {};
	
	virtual ~IIOHandlerInterface() {};

	bool bConnected = false;
	
	FOnMessageReceived OnMessageReceived;
protected:
	MessageQueuePtr MessageQueue = nullptr;
	HttpMessageQueuePtr HttpMessageQueue = nullptr;
};
