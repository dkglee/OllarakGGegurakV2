// Fill out your copyright notice in the Description page of Project Settings.


#include "HouseOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AHouseOneProp::AHouseOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-House-01.SM-House-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1020"));
}

// Called when the game starts or when spawned
void AHouseOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHouseOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

