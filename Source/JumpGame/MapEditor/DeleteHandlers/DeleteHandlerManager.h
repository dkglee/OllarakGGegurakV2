// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "DeleteHandlerManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UDeleteHandlerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UDeleteHandlerManager();

	void HandleDelete(FClickResponse& ClickResponse);

protected:
	virtual void BeginPlay() override;

};
