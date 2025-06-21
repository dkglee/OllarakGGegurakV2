// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkGameMode.h"
#include "JumpGame/Core/GameState/TypeInfo/GameInfo.h"
#include "MapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API AMapGameMode : public ANetworkGameMode
{
	GENERATED_BODY()

public:
	AMapGameMode();
	
protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapLoad", meta = (AllowPrivateAccess = "true"))
	class ULoadMapComponent* LoadMapComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapLoad", meta = (AllowPrivateAccess = "true"))
	class USaveMapComponent* SaveMapComponent = nullptr;

public:
	UPROPERTY()
	int32 PlayerIdx = 0;
	
	UPROPERTY()
	class UJumpGameInstance* GI;
};
