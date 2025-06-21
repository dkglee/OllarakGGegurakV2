// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmThreeProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFarmThreeProp::AFarmThreeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Farm-03.SM-Farm-03"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1044"));
}

// Called when the game starts or when spawned
void AFarmThreeProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFarmThreeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

