// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameState.h"
#include "TypeInfo/GameInfo.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API ALobbyGameState : public ANetworkGameState
{
	GENERATED_BODY()

public:
	ALobbyGameState();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnClientAdded(const FString& NetID) override;
	virtual void OnConnectionSucceeded() override;

	UPROPERTY()
	class UJumpGameInstance* GI;
	
	// 플레이어가 들어올때마다 보내줄 내용 (로비 이미지 업데이트)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateWaitImage(const FString& PlayerKey, const FPlayerInfo& PlayerInfo);

	// 업데이트 될 UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UWaitRoomUI> WaitRoomUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWaitRoomUI* WaitRoomUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoadMap")
	class ULoadMapComponent* LoadMapComponent;

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
};
