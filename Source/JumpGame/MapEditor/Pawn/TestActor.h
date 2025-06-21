// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "TestActor.generated.h"

UCLASS()
class JUMPGAME_API ATestActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	class UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	class UGridComponent* GridComponent = nullptr;
};
