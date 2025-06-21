// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPointProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ACheckPointProp::ACheckPointProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/checkpoint.checkpoint"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4003"));
}

// Called when the game starts or when spawned
void ACheckPointProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckPointProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

