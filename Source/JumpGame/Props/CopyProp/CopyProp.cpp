// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ACopyProp::ACopyProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 끄기
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComp->SetCollisionProfileName(TEXT("OverlapProp"));
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
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

