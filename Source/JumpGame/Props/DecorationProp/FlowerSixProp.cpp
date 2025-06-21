// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerSixProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFlowerSixProp::AFlowerSixProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Flower-06.SM-Flower-06"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4015"));
}

// Called when the game starts or when spawned
void AFlowerSixProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowerSixProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

