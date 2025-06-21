// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTileThreeProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AGroundTileThreeProp::AGroundTileThreeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/ground_02_tile.ground_02_tile"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(3, 3, 2));

	PropDataComponent->SetPropID(TEXT("1019"));
}

// Called when the game starts or when spawned
void AGroundTileThreeProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundTileThreeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

