// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyProp.h"


// Sets default values
ACopyProp::ACopyProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 끄기
}

// Called when the game starts or when spawned
void ACopyProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACopyProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

