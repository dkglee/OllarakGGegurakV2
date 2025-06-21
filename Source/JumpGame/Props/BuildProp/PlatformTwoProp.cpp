// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTwoProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
APlatformTwoProp::APlatformTwoProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/platform_02.platform_02"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SetSize(FVector(4, 2, 1));

	PropDataComponent->SetPropID(TEXT("1027"));
}

// Called when the game starts or when spawned
void APlatformTwoProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformTwoProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

