// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketManageComponent.h"

#include "Message.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values for this component's properties
UWebSocketManageComponent::UWebSocketManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWebSocketManageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	TWeakObjectPtr<UWebSocketManageComponent> WeakThis = this;
	
	Socket->OnConnected().AddLambda([WeakThis]() {
		if (!WeakThis.IsValid()) return;

		UWebSocketManageComponent* StrongThis = WeakThis.Get();
		
		UE_LOG(LogTemp, Warning, TEXT("Connected to WebSocket server."));
		StrongThis->SendPingMessage();
	});

	Socket->OnConnectionError().AddLambda([WeakThis](const FString& Error) {
		if (!WeakThis.IsValid()) return;

		UWebSocketManageComponent* StrongThis = WeakThis.Get();
		
		UE_LOG(LogTemp, Error, TEXT("Connection error: %s"), *Error);
	});

	// Socket->OnMessage().AddLambda([WeakThis](const FString& Message) {
	// 	if (!WeakThis.IsValid()) return;
	//
	// 	UWebSocketManageComponent* StrongThis = WeakThis.Get();
	// 	
	// 	UE_LOG(LogTemp, Log, TEXT("Received message: %s"), *Message);
	// });

	Socket->OnRawMessage().AddLambda([WeakThis](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) {
		if (!WeakThis.IsValid()) return;

		UWebSocketManageComponent* StrongThis = WeakThis.Get();
		
		StrongThis->OnMessageReceived(Data, Size, BytesRemaining);
	});

	Socket->Connect();
}

void UWebSocketManageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Type::Destroyed)
	{
		if (Socket) Socket->Close();
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UWebSocketManageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWebSocketManageComponent::SendPongMessage()
{
	double TimeStamp = FPlatformTime::Seconds();
	
	FMessageUnion PongMessage;
	PongMessage.PongMessage.Header.Type = EMessageType::Pong;
	PongMessage.PongMessage.Header.PayloadSize = sizeof(FEyeTrackingNotifyMessage);
	PongMessage.PongMessage.Header.PlayerID = 1;
	PongMessage.PongMessage.Header.SessionID[0] = 1;
	PongMessage.PongMessage.TimeStamp = TimeStamp;

	Socket->Send(PongMessage.RawData, PongMessage.PongMessage.Header.PayloadSize, true);
}

void UWebSocketManageComponent::SendPingMessage()
{
	double TimeStamp = FPlatformTime::Seconds();

	FMessageUnion PingMessage;
	PingMessage.PingMessage.Header.Type = EMessageType::Ping;
	PingMessage.PingMessage.Header.PayloadSize = sizeof(FPingMessage);
	PingMessage.PingMessage.Header.PlayerID = 1;
	PingMessage.PingMessage.Header.SessionID[0] = 1;
	PingMessage.PingMessage.TimeStamp = TimeStamp;

	Socket->Send(PingMessage.RawData, PingMessage.PingMessage.Header.PayloadSize, true);
}

void UWebSocketManageComponent::OnMessageReceived(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	const uint8* RawData = (const uint8*)Data;

	FMessageUnion Message;
	// memory copy
	FMemory::Memcpy(&Message, RawData, Size);

	switch (Message.Header.Type)
	{
	case EMessageType::Ping:
		FFastLogger::LogConsole(TEXT("Ping TimeStamp : %f"), Message.PingMessage.TimeStamp);
		SendPongMessage();
		break;
	case EMessageType::Pong:
		FFastLogger::LogConsole(TEXT("Pong TimeStamp : %f"), Message.PongMessage.TimeStamp);
		SendPingMessage();
		break;
	default:
		FFastLogger::LogConsole(TEXT("Unknown message type"));
		break;
	}
}
