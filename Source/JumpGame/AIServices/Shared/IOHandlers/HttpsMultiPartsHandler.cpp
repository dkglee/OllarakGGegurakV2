// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpsMultiPartsHandler.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JumpGame/AIServices/Shared/HttpMessage.h"
#include "JumpGame/Utils/FastLogger.h"


FHttpsMultiPartsHandler::FHttpsMultiPartsHandler()
{
}

FHttpsMultiPartsHandler::~FHttpsMultiPartsHandler()
{
}

bool FHttpsMultiPartsHandler::Init(const FIOHandlerInitInfo& InitInfo, MessageQueuePtr InMessageQueuePtr, HttpMessageQueuePtr InHttpMessageQueuePtr)
{
	MessageQueue = InMessageQueuePtr;
	HttpMessageQueue = InHttpMessageQueuePtr;
	return true;
}

FString FHttpsMultiPartsHandler::GenerateBoundary()
{
	return FString::Printf(TEXT("----UEBoundary-%d"), FMath::Rand());
}

FString FHttpsMultiPartsHandler::BuildMultipartBody(const FHttpMultipartRequest& Request, const FString& Boundary,
	TArray<uint8>& OutBody)
{
	OutBody.Reset();

	const FString LineBreak = TEXT("\r\n");
	TArray<uint8> TempBody;

	for (const auto& Field : Request.MultipartFields)
	{
		FString Header = TEXT("--") + Boundary + LineBreak;
		Header += FString::Printf(TEXT("Content-Disposition: form-data; name=\"%s\""), *Field.FieldName);

		if (!Field.FileName.IsEmpty())
		{
			Header += FString::Printf(TEXT("; filename=\"%s\""), *Field.FileName);
		}

		Header += LineBreak;
		Header += FString::Printf(TEXT("Content-Type: %s"), *Field.ContentType);
		Header += LineBreak + LineBreak;

		FTCHARToUTF8 HeaderUtf8(*Header);
		TempBody.Append((const uint8*)HeaderUtf8.Get(), HeaderUtf8.Length());
		TempBody.Append(Field.Data);
		TempBody.Append((const uint8*)TCHAR_TO_UTF8(*LineBreak), LineBreak.Len());
	}

	// 마지막 boundary
	FString Footer = TEXT("--") + Boundary + TEXT("--") + LineBreak;
	FTCHARToUTF8 FooterUtf8(*Footer);
	TempBody.Append((const uint8*)FooterUtf8.Get(), FooterUtf8.Length());

	OutBody = MoveTemp(TempBody);
	return Boundary;
}

// 저 콜백을 사용해야 함.
bool FHttpsMultiPartsHandler::SendGameMessage(const FHttpMessageWrapper& HttpMessage)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	const FHttpMultipartRequest* MultipartRequest = std::get_if<FHttpMultipartRequest>(&HttpMessage.HttpMessage);
	if (!MultipartRequest)
	{
		FFastLogger::LogConsole(TEXT("Invalid HttpMessage type"));
		return false;
	}
	
	// 서버 URL + 경로
	FString FullURL = MultipartRequest->ServerURL + MultipartRequest->RequestPath;
	FFastLogger::LogConsole(TEXT("FullURL: %s"), *FullURL);
	HttpRequest->SetURL(FullURL);
	HttpRequest->SetVerb(TEXT("POST"));

	// Content-Type 설정
	FString Boundary = GenerateBoundary();
	FString ContentType = FString::Printf(TEXT("multipart/form-data; boundary=%s"), *Boundary);
	HttpRequest->SetHeader(TEXT("Content-Type"), ContentType);

	// 기타 헤더 설정
	for (const auto& Pair : MultipartRequest->AdditionalHeaders)
	{
		HttpRequest->SetHeader(Pair.Key, Pair.Value);
	}

	// Body 생성
	TArray<uint8> RequestBody;
	BuildMultipartBody(*MultipartRequest, Boundary, RequestBody);
	HttpRequest->SetContent(RequestBody);

	// 응답 처리
	HttpRequest->OnProcessRequestComplete().BindSP(AsShared(), &FHttpsMultiPartsHandler::OnHttpRequestComplete);

	HttpRequest->ProcessRequest();

	return true;
}

void FHttpsMultiPartsHandler::OnHttpRequestComplete(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess)
{
	if (!this)
	{
		return;
	}
	
	if (bSuccess && Resp.IsValid())
	{
		FFastLogger::LogConsole(TEXT("Response Code: %d"), Resp->GetResponseCode());
		FFastLogger::LogConsole(TEXT("Response: %s"), *Resp->GetContentAsString());

		// 헤더 설정
		FHttpMessageWrapper ResponseMessage;
		ResponseMessage.Header.Type = EMessageType::HttpMultipartResponse;

		// 내용 담기
		ResponseMessage.HttpMessage = FHttpMultipartResponse(); // 직접 할당

		FHttpMultipartResponse& HttpMultipartResponseRef = std::get<FHttpMultipartResponse>(ResponseMessage.HttpMessage);

		HttpMultipartResponseRef.ResponseCode = Resp->GetResponseCode();
		HttpMultipartResponseRef.ResponseData = Resp->GetContent();
		HttpMultipartResponseRef.ResponseText = Resp->GetContentAsString();

		if (HttpMessageQueue && HttpMessageQueue->find(EMessageType::HttpMultipartResponse) != HttpMessageQueue->end())
		{
			HttpMessageQueue->at(EMessageType::HttpMultipartResponse).push(ResponseMessage);
		}
		else
		{
			FFastLogger::LogConsole(TEXT("Response type not found in queue"));
		}

		OnMessageReceived.Broadcast(true);
	}
	else
	{
		FFastLogger::LogConsole(TEXT("HTTP Request Failed : %s"), *Req->GetURL());
		FFastLogger::LogConsole(TEXT("Error: %d"), Resp.IsValid() ? Resp->GetResponseCode() : -1);
		FFastLogger::LogConsole(TEXT("Error (Success): %d"), bSuccess);
		OnMessageReceived.Broadcast(false);
	}
}

bool FHttpsMultiPartsHandler::ReceiveMessage()
{
	return true;
}

void FHttpsMultiPartsHandler::SetReaderMode()
{
	IIOHandlerInterface::SetReaderMode();
}

void FHttpsMultiPartsHandler::SetWriterMode()
{
	IIOHandlerInterface::SetWriterMode();
}
