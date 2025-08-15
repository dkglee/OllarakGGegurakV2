// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "ClientRoomGameState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AClientRoomGameState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	AClientRoomGameState();
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConnectionSucceeded() override;
	
public:
	UPROPERTY()
	class UJumpGameInstance* GI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFollower> FollowerUIClass;
	UPROPERTY(EditAnywhere)
	class UFollower* FollowerUI{nullptr};

	UPROPERTY(EditAnywhere)
	FVector2D PrevLoc;

	FVector2D PrevMousePos{FVector2D::ZeroVector};
	float AngleVelocity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoadMap")
	class ULoadMapComponent* LoadMapComponent;

public:
	// LobbyGS -> ClientRoomGS
	// 업데이트 될 UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UClientRoomUI> ClientRoomUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UClientRoomUI* ClientRoomUI;
	
	// 1) 전송 시작 알림 (신뢰성 필요)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ClientBeginRecvImage(const FString& InMapName,
							  int32 InImageByteSize);        // 총 길이
	// 2) 본문 데이터 (대용량, 순서는 유지)
	UFUNCTION(NetMulticast, Reliable)                 // UE5=Order 속성, UE4는 자동순서
	void MulticastRPC_ClientRecvImageChunk(const TArray<uint8>& InChunk,
							  int32 InOffset);               // 이 Chunk가 쓰일 버퍼 오프셋
	// 3) 전송 완료 알림
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ClientEndRecvImage();

	UPROPERTY()
	TArray<uint8> CurrentMapImageData; //
	UPROPERTY()
	FString CurrentMapName; // 현재 맵 이름
	UPROPERTY()
	int32 RecvBytes; // 현재까지 받은 바이트 수

	UFUNCTION()
	class UTexture2D* MakeTextureFromBytes(const TArray<uint8>& FileData);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UOutroCinematic> OutroCinematicUIClass;
	UPROPERTY(EditAnywhere)
	class UOutroCinematic* OutroCinematic {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMediaSoundComponent* OutroSoundComponent;

	UFUNCTION()
	void OnOutroVideoEnd();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UEnding> EndingUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEnding* EndingUI;
};
