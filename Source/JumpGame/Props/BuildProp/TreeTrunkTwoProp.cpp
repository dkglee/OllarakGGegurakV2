// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeTrunkTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ATreeTrunkTwoProp::ATreeTrunkTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/tree_trunk_02.tree_trunk_02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1041"));
}

// Called when the game starts or when spawned
void ATreeTrunkTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeTrunkTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

