// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFarmOneProp::AFarmOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Farm-01.SM-Farm-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(3, 6, 1));

	PropDataComponent->SetPropID(TEXT("1042"));
}

// Called when the game starts or when spawned
void AFarmOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFarmOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

