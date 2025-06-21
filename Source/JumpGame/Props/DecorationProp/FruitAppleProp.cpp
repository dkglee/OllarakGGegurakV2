// Fill out your copyright notice in the Description page of Project Settings.


#include "FruitAppleProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFruitAppleProp::AFruitAppleProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/fruit.fruit"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4016"));
}

// Called when the game starts or when spawned
void AFruitAppleProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFruitAppleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

