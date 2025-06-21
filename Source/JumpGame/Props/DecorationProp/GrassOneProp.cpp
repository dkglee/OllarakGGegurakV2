// Fill out your copyright notice in the Description page of Project Settings.


#include "GrassOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AGrassOneProp::AGrassOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/grass_01.grass_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4017"));
}

// Called when the game starts or when spawned
void AGrassOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrassOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

