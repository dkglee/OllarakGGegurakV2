// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFlowerOneProp::AFlowerOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Flower-01.SM-Flower-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4010"));
}

// Called when the game starts or when spawned
void AFlowerOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowerOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

