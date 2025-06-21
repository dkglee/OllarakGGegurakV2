// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicProp.h"


// Sets default values
ALogicProp::ALogicProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Logic");
}

// Called when the game starts or when spawned
void ALogicProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALogicProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

