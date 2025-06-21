// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AFarmTwoProp::AFarmTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Kobo_ForestVillage/Meshes/SM-Farm-02.SM-Farm-02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(3, 4, 1));

	PropDataComponent->SetPropID(TEXT("1043"));
}

// Called when the game starts or when spawned
void AFarmTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFarmTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

