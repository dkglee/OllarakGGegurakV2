// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StageNodeInfo.h"
#include "GameFramework/Actor.h"
#include "StageNodeActor.generated.h"

UCLASS()
class JUMPGAME_API AStageNodeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageNodeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NodeID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasField = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FieldID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText FieldName;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TArray<AStageNodeActor*> ConnectedActors; // Actor 참조

	// 런타임 등록용
	FStageNodeInfo ToNodeInfo();
};
