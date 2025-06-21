// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ClientRoomGameMode.generated.h"

class UJumpGameInstance;
/**
 * 
 */
UCLASS()
class JUMPGAME_API AClientRoomGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnVideoEnd();
	virtual void BeginPlay() override;
	UPROPERTY()
	UJumpGameInstance* GI;
	
	// 업데이트 될 UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UClientRoomLogoUI> ClientRoomLogoUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClientRoomLogoUI* ClientRoomLogoUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UIntroCinematic> IntroCinematicUIClass;
	UPROPERTY(EditAnywhere)
	class UIntroCinematic* IntroCinematic {nullptr};
};
