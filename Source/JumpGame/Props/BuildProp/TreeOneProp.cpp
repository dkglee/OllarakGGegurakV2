// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ATreeOneProp::ATreeOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/tree_01.tree_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(4, 2, 4));

	PropDataComponent->SetPropID(TEXT("1032"));
}

// Called when the game starts or when spawned
void ATreeOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

