// Fill out your copyright notice in the Description page of Project Settings.


#include "RockTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ARockTwoProp::ARockTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/rock_02.rock_02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(4, 4, 2));

	PropDataComponent->SetPropID(TEXT("1029"));
}

// Called when the game starts or when spawned
void ARockTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

