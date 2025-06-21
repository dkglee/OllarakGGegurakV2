// Fill out your copyright notice in the Description page of Project Settings.


#include "CactusTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ACactusTwoProp::ACactusTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/cactus_02.cactus_02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(1, 1, 2));

	PropDataComponent->SetPropID(TEXT("1010"));
}

// Called when the game starts or when spawned
void ACactusTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACactusTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

