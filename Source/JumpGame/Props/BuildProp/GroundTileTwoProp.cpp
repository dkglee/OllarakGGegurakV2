// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTileTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AGroundTileTwoProp::AGroundTileTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/ground_02_tile.ground_02_tile"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 1));

	PropDataComponent->SetPropID(TEXT("1018"));
}

// Called when the game starts or when spawned
void AGroundTileTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundTileTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

