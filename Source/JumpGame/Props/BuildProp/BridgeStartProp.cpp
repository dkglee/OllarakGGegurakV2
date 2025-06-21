// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeStartProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ABridgeStartProp::ABridgeStartProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/bridge_start.bridge_start"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(1, 2, 2));;

	PropDataComponent->SetPropID(TEXT("1002"));
}

// Called when the game starts or when spawned
void ABridgeStartProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABridgeStartProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

