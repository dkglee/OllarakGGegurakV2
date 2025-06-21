// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
AKeyProp::AKeyProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/key.key"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4019"));
}

// Called when the game starts or when spawned
void AKeyProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

