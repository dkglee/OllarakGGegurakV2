// Fill out your copyright notice in the Description page of Project Settings.


#include "PathOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathOneProp::APathOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-01.SM-Path-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(4, 4, 1));

	PropDataComponent->SetPropID(TEXT("4021"));
}

// Called when the game starts or when spawned
void APathOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

