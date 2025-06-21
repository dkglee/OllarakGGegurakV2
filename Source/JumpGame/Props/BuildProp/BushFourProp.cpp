// Fill out your copyright notice in the Description page of Project Settings.


#include "BushFourProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ABushFourProp::ABushFourProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/bush_04.bush_04"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("1007"));
}

// Called when the game starts or when spawned
void ABushFourProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABushFourProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

