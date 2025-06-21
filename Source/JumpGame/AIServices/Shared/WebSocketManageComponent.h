// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "WebSocketManageComponent.generated.h"

/* GameState에 넣어서 사용하세요~ */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UWebSocketManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWebSocketManageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SendPongMessage();
	virtual void SendPingMessage();

	// @ Trouble
	// WebSocket은 서버가 /ws를 지정한 경우 뒤에 /ws를 붙여야 함
	const FString ServerURL = TEXT("ws://192.168.20.68:8029/ws");
	const FString ServerProtocol = TEXT("ws");

	// UPROPERTY()
	TSharedPtr<IWebSocket> Socket = nullptr;

private:
	// UFUNCTION()
	void OnMessageReceived(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
};
