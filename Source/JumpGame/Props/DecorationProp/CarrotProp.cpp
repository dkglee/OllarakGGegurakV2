// Fill out your copyright notice in the Description page of Project Settings.


#include "CarrotProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ACarrotProp::ACarrotProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Carrot-01.SM-Carrot-01"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	PropDataComponent->SetPropID(TEXT("4002"));
}

// Called when the game starts or when spawned
void ACarrotProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarrotProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

