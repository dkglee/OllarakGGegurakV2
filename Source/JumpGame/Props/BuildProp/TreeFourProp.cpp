// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeFourProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ATreeFourProp::ATreeFourProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Tree-02.SM-Tree-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(2, 2, 4));

	PropDataComponent->SetPropID(TEXT("1035"));
}

// Called when the game starts or when spawned
void ATreeFourProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeFourProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

