// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFourProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathFourProp::APathFourProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-04.SM-Path-04"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 1));

	PropDataComponent->SetPropID(TEXT("4024"));
}

// Called when the game starts or when spawned
void APathFourProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathFourProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

