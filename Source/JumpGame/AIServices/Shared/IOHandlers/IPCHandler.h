// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "IOHandlerInterface.h"
#include "JumpGame/AIServices/Shared/Message.h"

#define BUFFER_SIZE 1460
#define PIPE_PREFIX TEXT(R"(\\.\pipe\)")

class JUMPGAME_API FIPCHandler : public IIOHandlerInterface, public TSharedFromThis<FIPCHandler>
{
public:
	FIPCHandler();

	virtual bool Init(const FIOHandlerInitInfo& InitInfo, MessageQueuePtr InMessageQueuePtr, HttpMessageQueuePtr InHttpMessageQueuePtr) override;
	virtual bool SendGameMessage(const FMessageUnion& Message) override;
	virtual bool SendGameMessage(const FHttpMessageWrapper& HttpMessage) override { return false; };
	virtual bool ReceiveMessage() override;
	virtual void SetReaderMode() override { bReader = true; bWriter = false; }
	virtual void SetWriterMode() override { bReader = false; bWriter = true; };
	FString GetPipeName() { return PipeName; }
	
	virtual ~FIPCHandler() override;

private:
	bool ParseMessage(FMessageUnion& Message, DWORD BytesRead);
	
	FString PipeName = TEXT("");

	int32 BufferSize = BUFFER_SIZE;
	char Buffer[BUFFER_SIZE] = { 0 };

	size_t MaxCacheSize = BUFFER_SIZE * 10;
	uint8 CachedBuffer[BUFFER_SIZE * 10] = { 0 };
	size_t CachedLength = 0;
	
	void* Pipe = nullptr;

	bool bReader = false;
	bool bWriter = false;
};
