#pragma once

#include "CoreMinimal.h"
#include "IOHandlerInterface.h"
#include "Interfaces/IHttpRequest.h"

class JUMPGAME_API FHttpsMultiPartsHandler : public IIOHandlerInterface, public TSharedFromThis<FHttpsMultiPartsHandler>
{
public:
	FHttpsMultiPartsHandler();

	virtual bool Init(const FIOHandlerInitInfo& InitInfo, MessageQueuePtr InMessageQueuePtr, HttpMessageQueuePtr InHttpMessageQueuePtr) override;
	virtual bool SendGameMessage(const FMessageUnion& Message) override { return false; };
	virtual bool SendGameMessage(const FHttpMessageWrapper& HttpMessage) override;
	virtual bool ReceiveMessage() override;
	virtual void SetReaderMode() override;
	virtual void SetWriterMode() override;
	
	virtual ~FHttpsMultiPartsHandler() override;

	void OnHttpRequestComplete(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess);
private:
	FString GenerateBoundary();
	FString BuildMultipartBody(const FHttpMultipartRequest& Request, const FString& Boundary, TArray<uint8>& OutBody);
};
