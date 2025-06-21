// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFiveProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathFiveProp::APathFiveProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-05.SM-Path-05"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	SetSize(FVector(6, 4, 1));

	PropDataComponent->SetPropID(TEXT("4025"));
}

// Called when the game starts or when spawned
void APathFiveProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathFiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

