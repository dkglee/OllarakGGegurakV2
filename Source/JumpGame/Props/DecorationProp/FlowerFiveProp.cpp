// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerFiveProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFlowerFiveProp::AFlowerFiveProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Flower-05.SM-Flower-05"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4014"));
}

// Called when the game starts or when spawned
void AFlowerFiveProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowerFiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

