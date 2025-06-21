// Fill out your copyright notice in the Description page of Project Settings.


#include "RockOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ARockOneProp::ARockOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/rock_01.rock_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 2));

	PropDataComponent->SetPropID(TEXT("1028"));
}

// Called when the game starts or when spawned
void ARockOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

