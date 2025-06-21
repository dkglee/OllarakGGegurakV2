// Fill out your copyright notice in the Description page of Project Settings.


#include "CactusOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ACactusOneProp::ACactusOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/cactus_01.cactus_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1009"));
}

// Called when the game starts or when spawned
void ACactusOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACactusOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

