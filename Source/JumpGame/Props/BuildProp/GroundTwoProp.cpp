// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AGroundTwoProp::AGroundTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/ground_01.ground_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 1));

	PropDataComponent->SetPropID(TEXT("1015"));
}

// Called when the game starts or when spawned
void AGroundTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

