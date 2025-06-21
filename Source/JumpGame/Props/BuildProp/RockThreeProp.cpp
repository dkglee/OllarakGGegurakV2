// Fill out your copyright notice in the Description page of Project Settings.


#include "RockThreeProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ARockThreeProp::ARockThreeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/rock_03.rock_03"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(3, 3, 3));

	PropDataComponent->SetPropID(TEXT("1030"));
}

// Called when the game starts or when spawned
void ARockThreeProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockThreeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

