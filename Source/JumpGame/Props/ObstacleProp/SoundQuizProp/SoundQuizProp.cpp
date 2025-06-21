#include "SoundQuizProp.h"

#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "Misc/DateTime.h"
#include "JumpGame/AIServices/Shared/IOManagerComponent.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "Runtime/Core/Public/Containers/StringConv.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"


class UJumpGameInstance;
// Sets default values
ASoundQuizProp::ASoundQuizProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VoiceRecorderComponent = CreateDefaultSubobject<UVoiceRecorderComponent>(TEXT("VoiceRecorderComponent"));
	if (!VoiceRecorderComponent)
	{
		FFastLogger::LogConsole(TEXT("보이스 레코더 없습니다"));
	}

	// 여기서
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundAsset
	(TEXT("/Game/Sounds/Ques/SQ_QuizStart.SQ_QuizStart"));
	if (SoundAsset.Succeeded())
	{
		HitSound = Cast<USoundCue>(SoundAsset.Object);
	}
}

// Called when the game starts or when spawned
void ASoundQuizProp::BeginPlay()
{
	Super::BeginPlay();

	// TODO: 나중에는 오버랩으로 옮김 (더미용)
	NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	RisingWaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));

	PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

// Called every frame
void ASoundQuizProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 테스트용 키바인딩
	// if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
	// {
	// 	// 7번 누르면 퀴즈 메세지 받아오기
	// 	ReceiveSoundQuizMessage();
	// }
	// 	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
	// {
	// 	// 9번 누르면 퀴즈 음성 파일 메세지 전송
	// 	SendSoundQuizMessage();
	// }
	// if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
	// {
	// 	// 7번 누르면 퀴즈 메세지 받아오기
	// 	ReceiveSoundQuizMessage();
	// }

	// 테스트용 키바인딩
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
	{
		// 7번 누르면 녹음시작
		StartRecord();
	}
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
	{
		// 9번 누르면 녹음끝
		StopRecord();
	}

	// 치트키
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
	{
		// 8번 누르면 퀴즈 끝 메세지 전송
		SendEndSoundQuizNotify();
	}

	if (bIsMessageReceived)
	{
		// 메세지가 왔는지 확인하자
		ReceiveSoundQuizMessage();
	}
}

void ASoundQuizProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Frog")) return;
	if (bIsActive == false) return;
	
	if (!bIsOverlap)
	{
		// GameState 캐스팅
		NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());
		Character = Cast<AFrog>(OtherActor);

		// 캐릭터 퀴즈 카메라로 전환
		if (OtherActor->ActorHasTag(TEXT("Frog")))
		{
			// 모든 플레이어의 움직임 제한
			MulticastRPC_PlayerStopMovement();
		}
		GetWorldTimerManager().SetTimer(SoundTimerHandle, this, &ASoundQuizProp::StartSoundQuiz, 4.f, false);
		
		// 물 멈추자
		if (RisingWaterProp)
		{
			RisingWaterProp->StopRising();
		}
	}
}

void ASoundQuizProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
}

void ASoundQuizProp::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoundQuizProp ,bIsOverlap);
}

void ASoundQuizProp::MulticastRPC_PlayEffect(FVector Location, int32 Index)
{
	Character = Cast<AFrog>(PC->GetPawn());
	FVector CharcaterPos = Character->GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, CharcaterPos, 0.8f, 1.0f);
	this->PlayHitEffect(Index);
}

void ASoundQuizProp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SoundTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(RecordStartTimer);
		GetWorld()->GetTimerManager().ClearTimer(RecordStopTimer);
		GetWorld()->GetTimerManager().ClearTimer(SendFileMessage);
	}
	
	Super::EndPlay(EndPlayReason);
}

void ASoundQuizProp::SendStartSoundQuizNotify()
{
	FString Key;
	
	// 플레이어의 고유한 네트워크 ID값 가져오기
	const FUniqueNetIdRepl& NetIdRepl = PC->GetPlayerState<APlayerState>()->GetUniqueId();
	if (NetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
		// 문자열로 추출하자
		Key = NetId->ToString();
		
		// 게임 인스턴스 가져오기
		UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
		TMap<FString, FPlayerInfo>& InfoMap = GI->GetPlayerInfo();

		if (InfoMap.Contains(Key))
		{
			PlayerIdx = InfoMap[Key].PlayerID;
		}
		else
		{
			PlayerIdx = 0;
		}
	}
	
	// AI쪽에서 첫번째 문장을 준다면, 5초동안 녹음 후 전송
	// 퀴즈 시작 메시지를 보내주자
	FQuizNotifyMessage NotifyMessage;
	NotifyMessage.Header.PlayerID = PlayerIdx;
	NotifyMessage.Header.Type = EMessageType::QuizNotify;
	NotifyMessage.QuizID = 1;
	NotifyMessage.Start = 1; //시작
	NotifyMessage.End = 0;

	// 패킷 포장?
	// 전송할 데이터를 네트워크 전송에 적합한 형식으로 정리하는 것
	// 즉, 전송 가능한 구조로 만들자
	FMessageUnion Msg;
	FMemory::Memcpy(&Msg, &NotifyMessage, sizeof(NotifyMessage));
	Msg.Header = NotifyMessage.Header;
	Msg.Header.PayloadSize = sizeof(NotifyMessage);

	// 메세지 전송
	NetGS->IOManagerComponent->SendGameMessage(Msg);

	// 틱 활성화
	bIsMessageReceived = true;
}

void ASoundQuizProp::SendSoundQuizMessage()
{
	FFastLogger::LogScreen(FColor::Red,TEXT("음성파일 보냅니다"));
	
	FDateTime Now = FDateTime::Now();
	int32 Hour = Now.GetHour();
	int32 Minute = Now.GetMinute();
	int32 Second = Now.GetSecond();
	UE_LOG(LogTemp, Warning, TEXT("SendSoundQuizMessage 현재 시간: %02d:%02d:%02d"), Hour, Minute, Second);

	// 더미 데이터 만들어서 보내기 (음성 녹음 파일)
	// 1. 바이너리 데이터를 로딩하자 (처음 한번만!)
	if (CurrentSendIndex == 0)
	{
		// FString FilePath = TEXT("C:/FinalProject/Game/Saved/SoundQuizResponseFile.wav");
		FString FilePath = FPaths::ProjectDir() + TEXT("Saved/AudioRecordings/") + TEXT("SoundQuizResponseFile.wav");
		FFastLogger::LogFile(TEXT("SocketLog.txt"), TEXT("Sound FilePath : %s"), *FilePath);
		LoadWavFileBinary(FilePath, CachedBinaryWav);
		// 없으면 로그 출력
		if (CachedBinaryWav.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("WAV 로드 실패"));
			return;
		}
		// 총 WAV 데이터 크기를 저장해두고, 이후 인덱싱에 사용
		TotalWavSize = CachedBinaryWav.Num();
	}
	
	while (true)
	{
		// 2. 쪼개서 여러 패킷으로 전송 (1024바이트 단위로)
		// 현재 인덱스 기준으로 청크를 생성
		// WAV 전체에서 1024만큼 데이터를 꺼내고, 마지막 청크는 1024보다 작을 수 있으니까 Min()으로 크기 조절
		int32 CopySize = FMath::Min(ChunkSize, TotalWavSize - (CurrentSendIndex * ChunkSize));
		FWavRequestMessage ReqMessage;
		
		// 첫번째 패킷이면 start=1, 마지막 패킷이면 fin=1
		// 0으로 전체 초기화(패딩값넣기위해)
		FMemory::Memzero(&ReqMessage, sizeof(ReqMessage));
		ReqMessage.Header.PlayerID = PlayerIdx;
		ReqMessage.Header.Type = EMessageType::WaveRequest;
		ReqMessage.QuizID = 1;
		ReqMessage.Start = (CurrentSendIndex == 0) ? 1 : 0;
		ReqMessage.Index = CurrentSendIndex;
		ReqMessage.Fin = ((CurrentSendIndex + 1) * ChunkSize >= TotalWavSize) ? 1 : 0;
		ReqMessage.Size = TotalWavSize;
		// ReqMessage.ChunkSize = CopySize;
		
		// ChunkSize는 항상 1024를 넣는다
		ReqMessage.ChunkSize = ChunkSize;

		// RawData에 현재 인덱스 위치부터 CopySize만큼 WAV 데이터를 복사해 넣기
		FMemory::Memcpy(ReqMessage.RawData, CachedBinaryWav.GetData() + (CurrentSendIndex * ChunkSize), CopySize);

		// 패킷 포장
		FMessageUnion Msg;
		FMemory::Memcpy(&Msg, &ReqMessage, sizeof(ReqMessage));
		Msg.Header = ReqMessage.Header;
		Msg.Header.PayloadSize = sizeof(ReqMessage);

		// 메세지 전송
		NetGS->IOManagerComponent->SendGameMessage(Msg);

		// 다음 인덱스 준비
		CurrentSendIndex++;
		
		// Fin이면 타이머 종료 및 초기화
		if (ReqMessage.Fin)
		{
			FFastLogger::LogScreen(FColor::Red,TEXT("음성파일 전송완료"));
			UE_LOG(LogTemp, Log, TEXT("모든 WAV 패킷 전송 완료"));
			CurrentSendIndex = 0;
			TotalWavSize = 0;
			CachedBinaryWav.Empty();

			// 타이머 전부 초기화
			ClearAllTimer();
			return;
		}
	}
}

void ASoundQuizProp::ReceiveSoundQuizMessage()
{
	FMessageUnion RespMessage;
	// AI가 보내준 메세지를 받자
	if (!NetGS->IOManagerComponent->PopMessage(EMessageType::WaveResponse, RespMessage))
	{
		return;
	}

	// 내 메세지가 아니면 무시하자
	if (RespMessage.Header.PlayerID != PlayerIdx) return;

	// 내가 만든 변수에 넣자
	QuizID = RespMessage.WavResponseMessage.QuizID;
	Similarity = RespMessage.WavResponseMessage.Similarity;
	// 정답 여부 판단
	bSuccess = RespMessage.WavResponseMessage.bSuccess;
	// 먼저, 보내준 메세지에서 문자열의 길이를 추출한다 (몇글자니)
	FMemory::Memcpy(&MessageSize, RespMessage.WavResponseMessage.Message, sizeof(uint32));

	FFastLogger::LogConsole(TEXT("Payload Size: %d"), RespMessage.Header.PayloadSize);
	FFastLogger::LogConsole(TEXT("Received Message Size : %d"), MessageSize);

	// 그 다음, 문자열 길이가 0보다 크면 길이만큼 문자열을 복사해서 생성
	if (MessageSize > 0)
	{
		unsigned char* StrSize = RespMessage.WavResponseMessage.Message + sizeof(uint32);
		FUTF8ToTCHAR Converter(reinterpret_cast<const ANSICHAR*>(StrSize), MessageSize);
		MessageStr = FString(Converter.Length(), Converter.Get());
	}
	
	// 받은 메세지를 출력한다
	UE_LOG(LogTemp, Warning, TEXT("Receive QuizID: %d"), QuizID);
	UE_LOG(LogTemp, Warning, TEXT("Receive Similarity: %f"), Similarity);
	UE_LOG(LogTemp, Warning, TEXT("Receive BSuccess: %d"), bSuccess);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageSize: %d"), MessageSize);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageStr: %s"), *MessageStr);

	// 총 몇번 응답 했는지를 체크한다
	SendResponseIdx++;

	// 틱 비활성화
	bIsMessageReceived = false;
	
	if (SendResponseIdx >= 20)
	{
		UE_LOG(LogTemp, Warning, TEXT("퀴즈 실패"));
		GetWorld()->GetTimerManager().ClearTimer(RecordStartTimer);
	}
	else if (SendResponseIdx < 20)
	{
		if (bSuccess == 1 || Similarity*100 >= 89)
		{
			UE_LOG(LogTemp, Warning, TEXT("퀴즈 성공"));
			GetWorld()->GetTimerManager().ClearTimer(RecordStartTimer);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("퀴즈 녹음시작"));
			// 메세지를 받았다! 2초 후에 녹음 시작
			GetWorld()->GetTimerManager().SetTimer(RecordStartTimer, this, &ASoundQuizProp::StartRecord, 2.0f, false);
		}
	}
}

void ASoundQuizProp::SendEndSoundQuizNotify()
{
	// 퀴즈 끝 메시지를 보내주자
	FQuizNotifyMessage EndMessage;
	EndMessage.Header.PlayerID = PlayerIdx;
	EndMessage.Header.Type = EMessageType::QuizNotify;
	EndMessage.QuizID = 1;
	EndMessage.Start = 0;
	EndMessage.End = 1; //끝

	// 패킷 포장
	FMessageUnion Msg;
	FMemory::Memcpy(&Msg, &EndMessage, sizeof(EndMessage));
	Msg.Header = EndMessage.Header;
	Msg.Header.PayloadSize = sizeof(EndMessage);

	// 메세지 전송
	NetGS->IOManagerComponent->SendGameMessage(Msg);

	// 캐릭터 카메라 다시 원래대로 (끝난 사람만)
	Frog = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Frog)
	{
		Frog->ResumeMovement();
		Frog->CameraMovementMode();
		Frog->SetCrouchEnabled(true);
	}

	if (HasAuthority())
	{
		FinishedPlayerCount++;
		StartRisingWater();
	}
	else
	{
		// 서버 아니면, 서버한테 알려주기
		ServerRPC_EndPlayerCount();
	}

	// 퀴즈 정보 초기화
	ResetSoundQuiz();

	// 틱 비활성화
	bIsMessageReceived = false;
}

void ASoundQuizProp::ResetSoundQuiz()
{
	QuizID = -1;
	Similarity = 0.f;
	MessageSize = 0;
	MessageStr = "";

	SendResponseIdx = 0;
	
	CachedBinaryWav = { 0 };
	CurrentSendIndex = 0;
	TotalWavSize = 0;
}

void ASoundQuizProp::StartRecord()
{
	// 녹음 시작
	if (VoiceRecorderComponent)
	{
		VoiceRecorderComponent->StartRecording();
		FFastLogger::LogConsole(TEXT("녹음시작@@@@@@@@@@@@@@@@@@@@@@@"));
	}
	
	// 녹음 시작 3초뒤에 녹음 자동 종료
	GetWorld()->GetTimerManager().SetTimer(RecordStopTimer, this, &ASoundQuizProp::StopRecord, 3.0f, false);
}

void ASoundQuizProp::StopRecord()
{
	if (VoiceRecorderComponent)
	{
		VoiceRecorderComponent->StopRecording();
		FFastLogger::LogConsole(TEXT("녹음끝임@@@@@@@@@@@@@@@@@@@@@@@"));
	}

	// NOTE: 바로 보내면 안됨, 1초후에 전송
	// 녹음을 끝냈으니, 파일을 전송해주자 (1초후에)
	GetWorld()->GetTimerManager().SetTimer(SendFileMessage, this, &ASoundQuizProp::SendSoundQuizMessage, 1.0f, false);
	
	// 틱도 다시 활성화
	bIsMessageReceived = true;
}

// WAV 파일을 로드하고 바이너리 데이터로 전환
void ASoundQuizProp::LoadWavFileBinary(const FString& FilePath, TArray<uint8>& BinaryData)
{
    // WAV 파일을 로드
    if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
    {
       // 로드 실패 시
       UE_LOG(LogTemp, Error, TEXT("Failed to load WAV file from path: %s"), *FilePath);
       return;
    }
    // 로드 성공 시
    UE_LOG(LogTemp, Log, TEXT("Successfully loaded WAV file: %s"), *FilePath);
}

void ASoundQuizProp::MulticastRPC_PlayerStopMovement_Implementation()
{
	Frog = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Frog)
	{
		Frog->StopMovementAndResetRotation(FRotator::ZeroRotator);
		Frog->CameraMissionMode();
		Frog->SetCrouchEnabled(false);
		Frog->SetJumpGaugeVisibility(false);
	}
}

void ASoundQuizProp::ServerRPC_EndPlayerCount_Implementation()
{
	// 클라이언트는 퀴즈가 끝나면 이 함수를 호출한다
	FinishedPlayerCount++;

	UJumpGameInstance* GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	// TotalPlayerCount = GI->GetMaxPlayerCount();
	TotalPlayerCount = GetWorld()->GetGameState()->PlayerArray.Num();

	StartRisingWater();
}

void ASoundQuizProp::StartSoundQuiz()
{
	// GameState 캐스팅
	NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());

	if (HasAuthority() && Character->IsLocallyControlled())
	{
		MulticastRPC_PlayEffect(this->GetActorLocation());
	}
	else if (!HasAuthority() && Character->IsLocallyControlled())
	{
		Character->ServerRPC_ProcessOverlap(this);
	}
		
	// 이때 퀴즈 시작!
	SendStartSoundQuizNotify();
}

void ASoundQuizProp::StartRisingWater()
{
	// 모든 플레이어가 완료 되었을때만, 물이 차오른다
	if (FinishedPlayerCount >= TotalPlayerCount)
	{
		if (RisingWaterProp)
		{
			GetWorldTimerManager().SetTimer(WaterStartTimer, RisingWaterProp, &ARisingWaterProp::StartRising, 55.f, false);
			//RisingWaterProp->StartRising();
		}
	}
}

void ASoundQuizProp::ClearAllTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RecordStartTimer);
	GetWorld()->GetTimerManager().ClearTimer(RecordStopTimer);
	GetWorld()->GetTimerManager().ClearTimer(SendFileMessage);
	GetWorld()->GetTimerManager().ClearTimer(WaterStartTimer);
}
