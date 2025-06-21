// Fill out your copyright notice in the Description page of Project Settings.


#include "PathTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathTwoProp::APathTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-02.SM-Path-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4022"));
}

// Called when the game starts or when spawned
void APathTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

