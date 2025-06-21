#pragma once

#include <variant> // 반드시 필요

#include "Message.h"

struct FHttpMultipartField
{
	FString FieldName;
	FString FileName;
	FString ContentType;
	TArray<uint8> Data;

	FHttpMultipartField() = default;
	FHttpMultipartField(const FHttpMultipartField& Other) = default;
	FHttpMultipartField(FHttpMultipartField&& Other) noexcept = default;
	FHttpMultipartField& operator=(const FHttpMultipartField& Other) = default;
	FHttpMultipartField& operator=(FHttpMultipartField&& Other) noexcept = default;
};

struct FHttpMultipartRequest
{
	FString ServerURL;
	FString RequestPath;
	TMap<FString, FString> AdditionalHeaders;
	TArray<FHttpMultipartField> MultipartFields;
	TMap<FString, FString> QueryParams;

	FHttpMultipartRequest() = default;
	FHttpMultipartRequest(const FHttpMultipartRequest& Other) = default;
	FHttpMultipartRequest(FHttpMultipartRequest&& Other) noexcept = default;
	FHttpMultipartRequest& operator=(const FHttpMultipartRequest& Other) = default;
	FHttpMultipartRequest& operator=(FHttpMultipartRequest&& Other) noexcept = default;
};

struct FHttpMultipartResponse
{
	int32 ResponseCode;
	TArray<uint8> ResponseData;
	FString ResponseText;

	FHttpMultipartResponse() = default;
	FHttpMultipartResponse(const FHttpMultipartResponse& Other) = default;
	FHttpMultipartResponse(FHttpMultipartResponse&& Other) noexcept = default;
	FHttpMultipartResponse& operator=(const FHttpMultipartResponse& Other) = default;
	FHttpMultipartResponse& operator=(FHttpMultipartResponse&& Other) noexcept = default;
};

// std::variant 기반 타입 정의
using FHttpMessageVariant = std::variant<FHttpMultipartRequest, FHttpMultipartResponse>;

struct FHttpMessageWrapper 
{
	FMessageHeader Header;
	FHttpMessageVariant HttpMessage;

	// 기본 생성자
	FHttpMessageWrapper()
		: Header()
		, HttpMessage(FHttpMultipartRequest()) // std::variant는 rvalue 생성 가능
	{}

	// 생성자 오버로드 - Request
	explicit FHttpMessageWrapper(const FHttpMultipartRequest& InRequest)
		: Header()
		, HttpMessage(FHttpMultipartRequest(InRequest)) // 복사본 생성
	{}

	// 생성자 오버로드 - Response
	explicit FHttpMessageWrapper(const FHttpMultipartResponse& InResponse)
		: Header()
		, HttpMessage(FHttpMultipartResponse(InResponse))
	{}

	// 복사 생성자
	FHttpMessageWrapper(const FHttpMessageWrapper& Other)
		: Header(Other.Header)
		, HttpMessage(Other.HttpMessage)
	{}

	// Move 생성자
	FHttpMessageWrapper(FHttpMessageWrapper&& Other) noexcept
		: Header(MoveTemp(Other.Header))
		, HttpMessage(MoveTemp(Other.HttpMessage))
	{}

	// 복사 대입 연산자
	FHttpMessageWrapper& operator=(const FHttpMessageWrapper& Other)
	{
		if (this != &Other)
		{
			Header = Other.Header;
			HttpMessage = Other.HttpMessage;
		}
		return *this;
	}

	// Move 대입 연산자
	FHttpMessageWrapper& operator=(FHttpMessageWrapper&& Other) noexcept
	{
		if (this != &Other)
		{
			Header = MoveTemp(Other.Header);
			HttpMessage = MoveTemp(Other.HttpMessage);
		}
		return *this;
	}
};
