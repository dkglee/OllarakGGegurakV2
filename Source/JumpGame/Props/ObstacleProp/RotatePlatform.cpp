// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatePlatform.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ARotatePlatform::ARotatePlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("RotatePlatform");
	
	MeshComp->SetRelativeScale3D(FVector(5,5,1));
	CollisionComp->SetBoxExtent(FVector(250,250,50));

	Super::SetSize(FVector(5,5,1));

	PropDataComponent->SetPropID(TEXT("3001"));
}

// Called when the game starts or when spawned
void ARotatePlatform::BeginPlay()
{
	Super::BeginPlay();

	RotAxis = FRotator(0,1,0);
	RotAngle = 100.f;
}

// Called every frame
void ARotatePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
