// Fill out your copyright notice in the Description page of Project Settings.


#include "PathThreeProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathThreeProp::APathThreeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-03.SM-Path-03"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(4, 6, 1));

	PropDataComponent->SetPropID(TEXT("4023"));
}

// Called when the game starts or when spawned
void APathThreeProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathThreeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

