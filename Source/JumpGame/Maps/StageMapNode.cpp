// Fill out your copyright notice in the Description page of Project Settings.


#include "StageMapNode.h"


// Sets default values
AStageMapNode::AStageMapNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStageMapNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageMapNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

