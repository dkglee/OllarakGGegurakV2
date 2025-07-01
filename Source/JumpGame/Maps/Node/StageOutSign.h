// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageOutSign.generated.h"

UCLASS()
class JUMPGAME_API AStageOutSign : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageOutSign();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SignMesh;

	// 파티클
	UPROPERTY(EditAnywhere)
	class UParticleSystem* SignEffect;

	UFUNCTION()
	void SpawnSign(bool bIsZero);

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
};

