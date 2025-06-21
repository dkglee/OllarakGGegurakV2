// Fill out your copyright notice in the Description page of Project Settings.


#include "IPCHandler.h"

#include "JumpGame/Utils/FastLogger.h"
/*
 * 이 오류는 RHITransientResourceAllocator.h 파일에서 max 매크로 충돌이 발생했기 때문에 생긴 것입니다. Visual Studio나 Windows 헤더에서는 흔히 <windows.h>가 max와 min을 매크로로 정의하기 때문에, C++ 표준 라이브러리의 std::max / std::min과 충돌하게 됩니다.
 */
#define WIN32_LEAN_AND_MEAN
#define NORESOURCE      // UpdateResource 함수 제거 (중요!)
#define NOMINMAX
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"

FIPCHandler::FIPCHandler()
{
}

bool FIPCHandler::Init(const FIOHandlerInitInfo& InitInfo, MessageQueuePtr InMessageQueuePtr, HttpMessageQueuePtr InHttpMessageQueuePtr)
{
	PipeName = bReader ? PIPE_PREFIX + InitInfo.ReadPipeName : PIPE_PREFIX + InitInfo.SendPipeName;
	MessageQueue = InMessageQueuePtr;

	const char* PipeNameA = TCHAR_TO_ANSI(*PipeName);
	
	//FFastLogger::LogConsole(TEXT("Connecting To Pipe : %s"), *PipeName);
	Pipe = CreateFileA(
		PipeNameA,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
		);

	// TODO : 실패에 대한 처리가 필요함
	// 만약 연결이 실패했다면
	if (Pipe == INVALID_HANDLE_VALUE) {
		// FFastLogger::LogConsole(TEXT("Failed to connect to pipe: %s"), *PipeName);

		return false;
	}

	DWORD mode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
	BOOL success = SetNamedPipeHandleState(Pipe, &mode, NULL, NULL);
	if (!success) {
		//FFastLogger::LogConsole(TEXT("Failed to open pipe as non-blocking"));
		return false;
	}
	
	FFastLogger::LogConsole(TEXT("Succeeded to connect to pipe: %s"), *PipeName);
	bConnected = true;
	return true;
}

bool FIPCHandler::SendGameMessage(const FMessageUnion& Message)
{
	if (!bConnected)
	{
		// FFastLogger::LogConsole(TEXT("Not connected to pipe: %s"), *PipeName);
		return false;
	}
	DWORD BytesWritten;
	if (!WriteFile(Pipe, Message.RawData, Message.Header.PayloadSize, &BytesWritten, NULL))
	{
		return false;
	}
	if (BytesWritten != Message.Header.PayloadSize)
	{
		// FFastLogger::LogConsole(TEXT("Failed to send message: %s"), *PipeName);
		return false;
	}
	return true;
}

bool FIPCHandler::ReceiveMessage()
{
	if (!bConnected)
	{
		//FFastLogger::LogConsole(TEXT("Not connected to pipe: %s"), *PipeName);
		return false;
	}
	DWORD BytesRead;

	BOOL Result = ReadFile(Pipe, Buffer, BufferSize, &BytesRead, NULL);
	if (!Result)
	{
		if (GetLastError() != ERROR_NO_DATA)
		{
			return false;
		}
		// FFastLogger::LogConsole(TEXT("Failed to read message: %s"), *PipeName);
	}
	
	// 1. 버퍼에 읽은 내용을 누적 저장 (문자열 아님!)
	if (CachedLength + BytesRead > MaxCacheSize)
	{
		FFastLogger::LogConsole(TEXT("Buffer overflow detected: %s"), *PipeName);
		return false;
	}
	
	FMemory::Memcpy(CachedBuffer + CachedLength, Buffer, BytesRead);
	CachedLength += BytesRead;

	// FFastLogger::LogConsole(TEXT("Cached Length : %d"), CachedLength);
	// 2. 메시지 파싱
	while (true)
	{
		FMessageUnion Message;
		if (!ParseMessage(Message, BytesRead))
		{
			// FFastLogger::LogConsole(TEXT("Failed to parse message: %s"), *PipeName);
			break ;
		}

		// 3. 처리 완료
		if (MessageQueue && MessageQueue->find(Message.Header.Type) != MessageQueue->end())
		{
			//FFastLogger::LogConsole(TEXT("Add Message to Queue"));
			MessageQueue->at(Message.Header.Type).push(Message);
		}
		else
		{
			FFastLogger::LogConsole(TEXT("Message type not found in queue: %s"), *PipeName);
		}
	}
	
	return true;
}

FIPCHandler::~FIPCHandler()
{
	CloseHandle(Pipe);
}

bool FIPCHandler::ParseMessage(FMessageUnion& Message, DWORD BytesRead)
{
	// 1. 최소한 헤더는 있어야 함
	if (CachedLength < sizeof(FMessageHeader))
		return false;

	// 2. 헤더 파싱
	FMessageHeader Header;
	FMemory::Memcpy(&Header, CachedBuffer, sizeof(FMessageHeader));	

	// 3. 전체 메시지 길이 계산
	size_t TotalMessageSize = Header.PayloadSize;

	// 4. 전체 메시지가 도착했는지 확인
	if (CachedLength < TotalMessageSize)
		return false;

	// 5. 메시지 내용 파싱
	FMemory::Memcpy(&Message, CachedBuffer, Header.PayloadSize);

	// 6. 처리된 메시지만큼 뒤로 밀기
	size_t RemainingLength = CachedLength - TotalMessageSize;
	if (RemainingLength > 0)
	{
		std::memmove(CachedBuffer, CachedBuffer + TotalMessageSize, RemainingLength);
	}
	CachedLength = RemainingLength;
	
	return true;
}
