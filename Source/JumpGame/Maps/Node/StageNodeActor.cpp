// Fill out your copyright notice in the Description page of Project Settings.


#include "StageNodeActor.h"


// Sets default values
AStageNodeActor::AStageNodeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStageNodeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageNodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FStageNodeInfo AStageNodeActor::ToNodeInfo()
{
	FStageNodeInfo Info;
	Info.NodeID = NodeID;
	Info.WorldPosition = GetActorLocation();
	Info.bHasField = bHasField;
	Info.FieldID = FieldID;
	Info.FieldName = FieldName;

	/*for (AStageNodeActor* Connected : ConnectedActors)
	{
		if (Connected)
		{
			Info.ConnectedNodes.Add(Connected->NodeID);
		}
	}*/

	return Info;
}

