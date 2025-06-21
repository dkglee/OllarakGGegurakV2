// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConnectionVerifyComponent.generated.h"

// Connection이 연결되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionSucceeded);
// Connection이 차단되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionBlocked);
// 서버 기준 Connection이 추가 되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientAdded, const FString&, NetID);
// 서버 기준 모든 클라이언트가 연결되었을 때 발생하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllClientAdded);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UConnectionVerifyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConnectionVerifyComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// 서버가 총 플레이어 수를 설정한다.
	// 이게 설정이 되어야 타이머가 작동한다.
	UFUNCTION()
	void InitMaxPlayerCount(const int32 InMaxPlayerCount);
	UFUNCTION()
	void AddClient(const FString& NetID);
	UFUNCTION()
	void ConfirmClient(const FString& NetID);
	UFUNCTION()
	void SetClientVerify(bool bCond);

	UPROPERTY()
	FOnConnectionSucceeded OnConnectionSucceeded;
	UPROPERTY()
	FOnConnectionBlocked OnConnectionBlocked;
	UPROPERTY()
	FOnClientAdded OnClientAdded;
	UPROPERTY()
	FOnAllClientAdded OnAllClientAdded;
	
private:
	UFUNCTION()
	void RecursiveCheckClientVerified();
	UFUNCTION()
	bool CheckAllClientAdded(TArray<FString>& UnVerifiedClients);
	
	UPROPERTY()
	bool bClientVerify = false;
	UPROPERTY()
	int32 MaxPlayerCount = -1;
	UPROPERTY()
	TMap<FString, bool> ClientMap;

	UPROPERTY()
	FTimerHandle ConnectionTimer;
	UPROPERTY()
	float CheckInterval = 0.5f;
};
