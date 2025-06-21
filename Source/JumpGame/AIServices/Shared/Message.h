#pragma once

#pragma pack(push, 1)

enum class EMessageType : uint8
{
	None = 0,
	Ping = 1,
	Pong = 2,
	QuizNotify = 3,
	WaveRequest = 4,
	WaveResponse = 5,
	EyeTrackingNotifyMessage = 6,
	EyeTrackingRequestMessage = 7,
	EyeTrackingResponseMessage = 8,
	HttpMultipartRequest = 9,
	HttpMultipartResponse = 10,
	ParsedWaveResponse = 100,
};

struct FMessageHeader
{
	EMessageType Type;
	uint16 PayloadSize;
	uint8 SessionID[100];
	uint8 PlayerID;
};

// Ping
struct FPingMessage
{
	FMessageHeader Header;
	double TimeStamp;
};

// Pong
struct FPongMessage
{
	FMessageHeader Header;
	double TimeStamp;
};

// QuizNotify
struct FQuizNotifyMessage
{
	FMessageHeader Header; // QuizNotify
		
	uint8 QuizID; // 퀴즈의 일련번호
	uint8 Start; // 0~1
	uint8 End; // 0~1
};

// WaveRequest
struct FWavRequestMessage {
	FMessageHeader Header; // WaveRequest
		 
	uint8 QuizID;
	uint8 Start; // 0 ~ 1
	uint32 Index; // 패킷의 순서
	uint8 Fin; // 0 ~ 1
	uint32 Size; // 총 음성 사이즈
	uint32 ChunkSize; // 청크 사이즈
	uint8 RawData[30720]; // 음성 바이너리 데이터
};

// WaveResponse
struct FWavResponseMessage {
	FMessageHeader Header; // WaveResponse
		
	uint8 QuizID;
	float Similarity; // 유사도
	uint8 bSuccess; // 정답
	// uint8 Answer;
	uint8 Message[1024]; // [len(str) : 4Byte, str]

	/*
		uint8 Message[1024];  // [len(str) : 4Byte, str]
	
		// 1. 먼저 문자열 길이(len)를 추출합니다.
		uint32 len = 0;
		FMemory::Memcpy(&len, Message, sizeof(uint32));  // Message의 처음 4바이트를 len에 복사
	
		// 2. 문자열을 추출하여 FString으로 변환합니다.
		FString str;
		if (len > 0)  // 문자열 길이가 0보다 클 때만
		{
		// Message + sizeof(uint32) 위치부터 길이만큼 복사하여 FString으로 생성
		str = FString(UTF8_TO_TCHAR(reinterpret_cast<char*>(Message + sizeof(uint32)), len));
		}
	*/
};

// EyeTrackingNotify
struct FEyeTrackingNotifyMessage {
	FMessageHeader Header; // EyetrackingNotify
		
	uint8 QuizID;
	uint8 SettingStart; // 감도 설정 요청할 때 1
	uint8 Start; // 0~1
	uint8 End; // 0~1
};

// EyeTrackingRequest
struct FEyeTrackingRequestMessage {
	FMessageHeader Header; // EyetrackingRequest
		
	uint8 QuizID;
	float Width; // 해상도 너비
	float Height; // 해상도 높이
	uint8 Start; // 1 ~ 4 ( 1 : 좌상단, 2 : 우상단, 3 : 우하단, 4 : 좌하단 )
	uint8 End;
};

// EyeTrackingResponse
struct FEyeTrackingResponseMessage {
	FMessageHeader Header;

	uint8 QuizID;		
	float X; // 좌표값 x
	float Y; // 좌표값 y
	uint8 bBlink; // 0~1(감았다: 1) 
	uint8 State; // 100 : 정상성, 200 : 클라 에러, 300 : 서버 에러
};


#pragma pack(pop)

union FMessageUnion
{
	FMessageHeader Header;
	FPingMessage PingMessage;
	FPongMessage PongMessage;
	FQuizNotifyMessage QuizNotifyMessage;
	FWavRequestMessage WavRequestMessage;
	FWavResponseMessage WavResponseMessage;
	FEyeTrackingNotifyMessage EyeTrackingNotifyMessage;
	FEyeTrackingRequestMessage EyeTrackingRequestMessage;
	FEyeTrackingResponseMessage EyeTrackingResponseMessage;

	uint8 RawData[1460];
};
