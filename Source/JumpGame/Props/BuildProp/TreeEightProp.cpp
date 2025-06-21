// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeEightProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ATreeEightProp::ATreeEightProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Tree-06.SM-Tree-06"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 6));

	PropDataComponent->SetPropID(TEXT("1039"));
}

// Called when the game starts or when spawned
void ATreeEightProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeEightProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

