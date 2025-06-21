// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketHandler.h"

#include "WebSocketsModule.h"
#include "JumpGame/Utils/FastLogger.h"

FSocketHandler::FSocketHandler()
{
}

bool FSocketHandler::Init(const FIOHandlerInitInfo& InitInfo, MessageQueuePtr InMessageQueuePtr, HttpMessageQueuePtr InHttpMessageQueuePtr)
{
	ServerURL = InitInfo.ServerUrl;
	ServerProtocol = InitInfo.ServerProtocol;
	MessageQueue = InMessageQueuePtr;
	// ...

	FFastLogger::LogConsole(TEXT("FSocketHandler::Init()"));
	FFastLogger::LogConsole(TEXT("ServerURL: %s"), *ServerURL);
	FFastLogger::LogConsole(TEXT("ServerProtocol: %s"), *ServerProtocol);
	
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	TWeakPtr<FSocketHandler> WeakSharedThis = AsShared();
	
	Socket->OnConnected().AddLambda([WeakSharedThis]() {
		if (!WeakSharedThis.IsValid()) return;

		TSharedPtr<FSocketHandler> StrongThis = WeakSharedThis.Pin();

		StrongThis->bConnected = true;

		UE_LOG(LogTemp, Warning, TEXT("Connected to WebSocket server."));
	});

	Socket->OnConnectionError().AddLambda([WeakSharedThis](const FString& Error) {
		if (!WeakSharedThis.IsValid()) return;

		TSharedPtr<FSocketHandler> StrongThis = WeakSharedThis.Pin();

		StrongThis->bConnected = false;

		// Retry Code
		// StrongThis->Socket->Connect();
		
		UE_LOG(LogTemp, Error, TEXT("Connection error: %s"), *Error);
	});
	
	Socket->OnRawMessage().AddLambda([WeakSharedThis](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) {
		if (!WeakSharedThis.IsValid()) return;

		TSharedPtr<FSocketHandler> StrongThis = WeakSharedThis.Pin();

		StrongThis->ReceiveSocketMessage(Data, Size, BytesRemaining);
	});

	Socket->Connect();
	
	return true;
}

bool FSocketHandler::SendGameMessage(const FMessageUnion& Message)
{
	if (!bConnected)
	{
		//UE_LOG(LogTemp, Error, TEXT("Not connected to WebSocket server."));
		return false;
	}

	// FFastLogger::LogScreen(FColor::Red, TEXT("Message Send : Size : %d"), Message.Header.PayloadSize);

	FFastLogger::LogConsole(TEXT("SendGameMessage() : %d"), Message.Header.PayloadSize);
	
	Socket->Send(Message.RawData, Message.Header.PayloadSize, true);
	return true;
}

void FSocketHandler::ReceiveSocketMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	const uint8* RawData = (const uint8*)Data;

	FMessageUnion Message;
	// memory copy
	FMemory::Memcpy(&Message, RawData, Size);
	FFastLogger::LogConsole(TEXT("Inside ReceiveSocketMessage"));
	if (MessageQueue && MessageQueue->find(Message.Header.Type) != MessageQueue->end())
	{
		FFastLogger::LogConsole(TEXT("Recieved Message (Size: %d)"), Size);
		MessageQueue->at(Message.Header.Type).push(Message);
	}
}

FSocketHandler::~FSocketHandler()
{
	
}
