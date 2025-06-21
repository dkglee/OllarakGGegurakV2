// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <queue>

#include "CoreMinimal.h"
#include "HTTPHandlerInitInfo.h"
#include "HttpMessage.h"
#include "Message.h"
#include "Components/ActorComponent.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "HttpManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UHttpManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHttpManagerComponent();

	bool SendHttpMessage(const FHttpMessageWrapper& HttpMessage);
	bool PopHttpMessage(const EMessageType& MessageType, FHttpMessageWrapper& OutMessage);
	
	TMap<EMessageType, TSharedPtr<class IIOHandlerInterface>> HttpHandlers;

	GETTER(FHTTPHandlerInitInfo, HttpHandlerInitInfo);
	
protected:
	UPROPERTY()
	FHTTPHandlerInitInfo HttpHandlerInitInfo;

	// Called when the game starts
	virtual void BeginPlay() override;
	void RegisterHttpHandler(const EMessageType& MessageType, TSharedPtr<class IIOHandlerInterface> Handler);
	bool InitializeConfigFile();
	
	std::map<EMessageType, std::queue<FHttpMessageWrapper>> HttpMessageQueue;
};
