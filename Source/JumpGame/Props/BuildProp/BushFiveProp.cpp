// Fill out your copyright notice in the Description page of Project Settings.


#include "BushFiveProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ABushFiveProp::ABushFiveProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/bush_05.bush_05"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(1, 1, 2));

	PropDataComponent->SetPropID(TEXT("1008"));
}

// Called when the game starts or when spawned
void ABushFiveProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABushFiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

