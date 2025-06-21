// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ACoinProp::ACoinProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/coin.coin"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	PropDataComponent->SetPropID(TEXT("4004"));
}

// Called when the game starts or when spawned
void ACoinProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoinProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

