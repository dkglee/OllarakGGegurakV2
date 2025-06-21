// Fill out your copyright notice in the Description page of Project Settings.


#include "LadderEndCapProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ALadderEndCapProp::ALadderEndCapProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/ladder_endcap.ladder_endcap"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4020"));
}

// Called when the game starts or when spawned
void ALadderEndCapProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadderEndCapProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

