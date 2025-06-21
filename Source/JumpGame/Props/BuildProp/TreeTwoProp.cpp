// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ATreeTwoProp::ATreeTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/tree_02.tree_02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	SetSize(FVector(2, 2, 4));

	PropDataComponent->SetPropID(TEXT("1033"));
}

// Called when the game starts or when spawned
void ATreeTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

