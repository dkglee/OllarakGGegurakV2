// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSixProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathSixProp::APathSixProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-06.SM-Path-06"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(6, 4, 1));

	PropDataComponent->SetPropID(TEXT("4026"));
}

// Called when the game starts or when spawned
void APathSixProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathSixProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

