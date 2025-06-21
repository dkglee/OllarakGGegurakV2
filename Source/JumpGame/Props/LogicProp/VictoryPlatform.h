// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "VictoryPlatform.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API AVictoryPlatform : public ALogicProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVictoryPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 골인지점에서 구현
	// void SpawnVictoryPlatform();

	UPROPERTY()
	FTimerHandle TombDropHandle;

	// 카메라 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* VictoryCamera;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// class UStaticMeshComponent* VictoryPlane;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VictoryTomb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VictoryTreeOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VictoryTreeTwo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* VictoryArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VictoryFlower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VictoryCarrot;

	// 조명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpotLightComponent* VictoryLight;
	
	// 1등 유저를 소환할 위치값을 반환하는 함수
	UFUNCTION()
	FVector SpawnVictoryCharacter();

public:
	// 연출용
	UFUNCTION()
	void DropProps();

	// 카메라 흔들림
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	float ShakeAmplitude = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	float ShakeFrequency = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	float ShakeDuration = 0.5f; // 흔들림 지속 시간
	UFUNCTION(BlueprintCallable)
	void StartCameraShake();

private:
	UPROPERTY()
	FVector OriginalCameraPos;
	UPROPERTY()
	bool bShouldShake = false;
	UPROPERTY()
	float ShakeTime = 0.f;
	UPROPERTY()
	float ElapsedShakeTime = 0.f; // 흔들림 누적 시간
};
