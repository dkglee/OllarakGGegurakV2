// Fill out your copyright notice in the Description page of Project Settings.


#include "LadderTileProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ALadderTileProp::ALadderTileProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/ladder_tile.ladder_tile"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	SetSize(FVector(1, 1, 2));

	PropDataComponent->SetPropID(TEXT("1024"));
}

// Called when the game starts or when spawned
void ALadderTileProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadderTileProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

