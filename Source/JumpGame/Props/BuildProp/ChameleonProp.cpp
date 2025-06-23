// Fill out your copyright notice in the Description page of Project Settings.


#include "ChameleonProp.h"


// Sets default values
AChameleonProp::AChameleonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChameleonProp::BeginPlay()
{
	Super::BeginPlay();

	CopyMaterial();
}

// Called every frame
void AChameleonProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChameleonProp::CopyMaterial()
{
	
}

