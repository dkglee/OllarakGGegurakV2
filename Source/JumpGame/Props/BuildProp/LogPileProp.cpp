// Fill out your copyright notice in the Description page of Project Settings.


#include "LogPileProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ALogPileProp::ALogPileProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-LogPile-01.SM-LogPile-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	
	SetSize(FVector(2, 2, 1));

	PropDataComponent->SetPropID(TEXT("1025"));
}

// Called when the game starts or when spawned
void ALogPileProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALogPileProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

