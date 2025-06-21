// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeFiveProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ATreeFiveProp::ATreeFiveProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Tree-03.SM-Tree-03"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 5));

	PropDataComponent->SetPropID(TEXT("1036"));
}

// Called when the game starts or when spawned
void ATreeFiveProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeFiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

