// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFlowerTwoProp::AFlowerTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Flower-02.SM-Flower-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4011"));
}

// Called when the game starts or when spawned
void AFlowerTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowerTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

