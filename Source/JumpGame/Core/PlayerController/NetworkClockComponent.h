// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "NetworkClockComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UNetworkClockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNetworkClockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	void RequestServerTime(APlayerController* Requester);

	// 서버 시간을 요청하는 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestServerTime(APlayerController* Requester, float RequestTime);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ReceiveServerTime(float InServerTime, float RequestTime);

	// 서버 시간을 가져온다.
	GETTER(float, ServerTime);

private:
	// 클라이언트가 가지고 있는 서버 시간
	UPROPERTY()
	float ServerTime = 0.0f;
};
