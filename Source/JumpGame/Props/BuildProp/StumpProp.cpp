// Fill out your copyright notice in the Description page of Project Settings.


#include "StumpProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AStumpProp::AStumpProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Stump-01.SM-Stump-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1031"));
}

// Called when the game starts or when spawned
void AStumpProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStumpProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

