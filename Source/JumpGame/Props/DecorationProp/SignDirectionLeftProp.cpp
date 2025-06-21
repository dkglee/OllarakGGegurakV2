// Fill out your copyright notice in the Description page of Project Settings.


#include "SignDirectionLeftProp.h"

#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ASignDirectionLeftProp::ASignDirectionLeftProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/sign_direction.sign_direction"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
	
	PropDataComponent->SetPropID(TEXT("4033"));
}

// Called when the game starts or when spawned
void ASignDirectionLeftProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASignDirectionLeftProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

