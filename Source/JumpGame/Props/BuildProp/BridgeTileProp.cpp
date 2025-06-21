// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeTileProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ABridgeTileProp::ABridgeTileProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/bridge_tile.bridge_tile"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(1, 2, 1));

	PropDataComponent->SetPropID(TEXT("1003"));
}

// Called when the game starts or when spawned
void ABridgeTileProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABridgeTileProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

