// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoiceRecorderComponent.h"
#include "JumpGame/Core/GameState/NetworkGameState.h"
#include "JumpGame/Props/ObstacleProp/ObstacleProp.h"
#include "SoundQuizProp.generated.h"

class ARisingWaterProp;

UCLASS()
class JUMPGAME_API ASoundQuizProp : public AObstacleProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundQuizProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	class ANetworkGameState* NetGS;
	UPROPERTY()
	ARisingWaterProp* RisingWaterProp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void MulticastRPC_PlayEffect(FVector Location, int32 Index = 0) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle SoundTimerHandle;
	
	// 퀴즈 한번만 실행
	UPROPERTY(Replicated)
	bool bIsOverlap = false;

	UPROPERTY()
	bool bIsClear = false;
	
	// 퀴즈 소켓 전송 관련
	UFUNCTION()
	virtual void SendStartSoundQuizNotify();
	UFUNCTION()
	virtual void SendSoundQuizMessage();
	UFUNCTION()
	virtual void ReceiveSoundQuizMessage();
	UFUNCTION()
	virtual void SendEndSoundQuizNotify();

	// 퀴즈 초기화
	UFUNCTION()
	virtual void ResetSoundQuiz();

	// 녹음 시작, 끝
	UFUNCTION()
	virtual void StartRecord();
	UFUNCTION()
	virtual void StopRecord();

	// 퀴즈 정보
	UPROPERTY()
	int32 QuizID = -1;
	UPROPERTY()
	float Similarity = 0.f;
	UPROPERTY()
	int32 bSuccess = 0;
	UPROPERTY()
	uint32 MessageSize = 0;
	UPROPERTY()
	FString MessageStr = "";
	// 퀴즈 전송할때마다 보내주자
	UPROPERTY()
	int32 PlayerIdx = 4;

	// 음성데이터를 몇번 주고 받으면 정답을 맞추지 못해도 게임을 종료
	UPROPERTY()
	uint32 SendResponseIdx = 0;
	// UPROPERTY()
	// TMap<int32, int32> ResponseCountPerPlayer;
	// UPROPERTY()
	// int32 MyResponseCount = 0;

public:
	// WAV 파일 로드 후 바이너리 데이터로 전환
	UPROPERTY()
	TArray<uint8> CachedBinaryWav;
	UPROPERTY()
	int32 CurrentSendIndex = 0;
	UPROPERTY()
	int32 TotalWavSize = 0;
	UPROPERTY()
	int32 ChunkSize = 30720;
	
	UFUNCTION()
	void LoadWavFileBinary(const FString& FilePath, TArray<uint8>& BinaryData);

	UPROPERTY(EditAnywhere)
	class AFrog* Frog;
	UPROPERTY()
	class APlayerController* PC;

protected:
	// 녹음 컴포넌트 붙이기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UVoiceRecorderComponent> VoiceRecorderComponent = nullptr;

	UPROPERTY()
	FTimerHandle RecordStartTimer;
	UPROPERTY()
	FTimerHandle RecordStopTimer;
	UPROPERTY()
	FTimerHandle SendFileMessage;

	// Tick을 활성화, 비활성화하는 변수
	UPROPERTY()
	bool bIsMessageReceived = false;

	// 동기화 관련
	UPROPERTY()
	int32 FinishedPlayerCount = 0;
	UPROPERTY()
	int32 TotalPlayerCount = 0;

	// 플레이어
	UPROPERTY()
	AFrog* Character = nullptr;

	UFUNCTION()
	virtual void StartSoundQuiz();
	UFUNCTION()
	void StartRisingWater();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayerStopMovement();
	UFUNCTION(Server, Reliable)
	void ServerRPC_EndPlayerCount();

private:
	UFUNCTION()
	void ClearAllTimer();

public:
	UPROPERTY()
	FTimerHandle WaterStartTimer;
};
