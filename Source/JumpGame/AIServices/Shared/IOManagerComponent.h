// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <queue>

#include "CoreMinimal.h"
#include "Message.h"
#include "Components/ActorComponent.h"
#include "IOHandlers/IOHandlerInitInfo.h"
#include "IOHandlers/IOHandlerInterface.h"
#include "IOManagerComponent.generated.h"

// GameState
// 클라든, 서버든 둘 다 통신을 해야하기 때문에
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UIOManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UIOManagerComponent();

	bool SendGameMessage(const FMessageUnion& Message);
	bool PopMessage(const EMessageType& MessageType, FMessageUnion& OutMessage);
	void RetryReadConnectToPipe(TSharedPtr<class FIPCHandler> IPCHandlerToRetry);
	void RetrySendConnectToPipe(TSharedPtr<class FIPCHandler> IPCHandlerToRetry);
	
protected:
	virtual void BeginPlay() override;
	
	void RegisterIOHandler(const EMessageType& MessageType, TSharedPtr<IIOHandlerInterface> Handler);
	bool InitializeConfigFile();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IO")
	FIOHandlerInitInfo IOHandlerInitInfo;
	
	// 어떤 통신 기법으로 보내겠다. (Socket, IPC 등)
	TMap<EMessageType, TSharedPtr<IIOHandlerInterface>> IOHandlers;
	std::map<EMessageType, std::queue<FMessageUnion>> MessageQueue;

	UPROPERTY()
	FTimerHandle RetryReadTimer;
	UPROPERTY()
	FTimerHandle RetrySendTimer;
	UPROPERTY()
	float RetryInterval = 0.5f;
};
