// Fill out your copyright notice in the Description page of Project Settings.


#include "PathEightProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathEightProp::APathEightProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-08.SM-Path-08"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(6, 2, 1));

	PropDataComponent->SetPropID(TEXT("4028"));
}

// Called when the game starts or when spawned
void APathEightProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathEightProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

