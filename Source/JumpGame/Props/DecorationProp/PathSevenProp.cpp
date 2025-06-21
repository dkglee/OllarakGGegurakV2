// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSevenProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APathSevenProp::APathSevenProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Path-07.SM-Path-07"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(5, 2, 1));
	
	PropDataComponent->SetPropID(TEXT("4027"));
}

// Called when the game starts or when spawned
void APathSevenProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathSevenProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

