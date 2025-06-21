// Fill out your copyright notice in the Description page of Project Settings.


#include "PotTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APotTwoProp::APotTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Pot-02.SM-Pot-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	PropDataComponent->SetPropID(TEXT("4031"));
}

// Called when the game starts or when spawned
void APotTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

