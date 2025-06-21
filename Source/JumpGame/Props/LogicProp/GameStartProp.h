// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogicProp.h"
#include "GameStartProp.generated.h"

UCLASS(Blueprintable)
class JUMPGAME_API AGameStartProp : public ALogicProp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameStartProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 플레이어 인덱스에 해당하는 시작 위치 반환
	UPROPERTY()
	int32 PlayerIdx = 0;
	FTransform PlayerStartTransform();

	/*// 테스트용: 혼자일때 되나 테스트
	FTransform SinglePlayerStart();*/
	
	UPROPERTY(VisibleAnywhere)
	TArray<USceneComponent*> StartPoints;

	// 얼마나 떨어뜨릴건지
	UPROPERTY(EditAnywhere,	BlueprintReadWrite)
	float Gap = 200.f;
};


