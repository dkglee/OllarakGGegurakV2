// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerFourProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFlowerFourProp::AFlowerFourProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Flower-04.SM-Flower-04"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4013"));
}

// Called when the game starts or when spawned
void AFlowerFourProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowerFourProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

