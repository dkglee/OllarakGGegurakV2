// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFenceOneProp::AFenceOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Fence-01.SM-Fence-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 1));

	PropDataComponent->SetPropID(TEXT("1012"));
}

// Called when the game starts or when spawned
void AFenceOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFenceOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

