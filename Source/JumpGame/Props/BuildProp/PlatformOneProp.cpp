// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformOneProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APlatformOneProp::APlatformOneProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/platform_01.platform_01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 1));

	PropDataComponent->SetPropID(TEXT("1026"));
}

// Called when the game starts or when spawned
void APlatformOneProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformOneProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

