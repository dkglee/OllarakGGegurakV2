// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeSixProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ATreeSixProp::ATreeSixProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Tree-04.SM-Tree-04"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	SetSize(FVector(2, 2, 4));

	PropDataComponent->SetPropID(TEXT("1037"));
}

// Called when the game starts or when spawned
void ATreeSixProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreeSixProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

